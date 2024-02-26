#pragma once
#include <tHeap.h>
#include <tTypeID.h>

static BarrelService* GlobalBarrelService;



//static BarrelService* GetGlobalBarrelService() { return GlobalBarrelService; }

BarrelNode* barrel_NodeLocation(BarrelService* service, int index)
{
	return index <= NONE || index >= service->Omegus._vector._count ? NULL :
		index == OMEGA ? &(service->Omegus) :
		&((BarrelNode*)(service->_heap->_heapStart))[index];
}
BarrelNode* barrel_LastPhysicalNode(BarrelService* service) { return barrel_NodeLocation(service, service->_lastPhysicalNode); }

int barrel_NextAvailableNode(BarrelService* service)
{
	if (!service)
	{
		PREENT("Service Failure!\n");
		return -1;
	}
		

	int next = service->_nextAvailable;

	if (next < 0)
	{
		PREENT("Getting Fresh Node...\n");
		next = service->Omegus._vector._count;
		barrel_VectorDeltaSizeRequest(service, &(service->Omegus), 1, NULL);
		//service->Omegus._requests++;
		//while (service->Omegus._requests > 0) { /* Call might hang? */ }
		PREENT("Fresh Node!\n");
	}
	else
	{
		BarrelNode* nextNode = barrel_NodeLocation(service, next);
		service->_nextAvailable = nextNode->_nextNode;
		PREENT("Recycled Node!\n");
	}

	return next;
}
int barrel_maximumAvailableBarrels(BarrelService* barrelService, int requested)
{
	int availableBarrels = heap_Remaining(barrelService->_heap) / sizeof(Barrel);
	return availableBarrels < requested ? availableBarrels : requested;
}

size_t barrel_VectorRemainingSizeCap(BarrelNode* node)
{
	return (node->_vector._capacity * sizeof(Barrel)) - (node->_vector._count * node->_vector._type->_size);
}

uint barrel_VectorRemainingUnitCap(BarrelNode* node)
{
	return barrel_VectorRemainingSizeCap(node) / node->_vector._type->_size;
}

bool barrel_DeltaSizeRequest(BarrelService* service, BarrelNode* node, int delta, void* source)
{
	if (delta == 0)
		return true;

	int deltaMag = delta < 0 ? -delta : delta;

	size_t remainingCapacity = barrel_VectorRemainingSizeCap(node);
	size_t deltaSize = deltaMag * node->_vector._type->_size;

	// determine initial block 'delta', rounding up if positive, to cover any trailing elements
	int requestDelta =
		delta > 0 ? ((deltaSize - remainingCapacity) / sizeof(Barrel)) + ((deltaSize - remainingCapacity) % sizeof(Barrel) > 0 ? 1 : 0) :
		delta < 0 ? ((remainingCapacity - deltaSize) / sizeof(Barrel)) : 0;

	// bound to a full block
	while (requestDelta % node->_vector._type->_barrelsPerBlock)
		requestDelta += requestDelta > 0 ? 1 : -1;

	// set request and wait for it to process
	node->_requests += requestDelta;
	while (node->_requests) { /* Call might hang... */ }

	node->_vector._count += delta;

	return true;
}

bool barrel_ResizeRequest(CollectionRequest request)
{
	BarrelNode* node = request._trg;
	return barrel_DeltaSizeRequest(request._service, node, request._count - node->_vector._count, request._src);
}

void* barrel_GetElementLocation(BarrelService* service, BarrelNode* node, uint index)
{
	ullong byteLength = node->_vector._capacity * sizeof(Barrel);
	ullong byteOffset = node->_barrelOffset * sizeof(Barrel);
	ullong byteIndex = index * node->_vector._type->_size;

	void* location = (void*)
		(((byteOffset + byteIndex) % byteLength) +
			(node->_barrelStart * sizeof(Barrel)) +
			(ullong)service->_heap->_heapStart);

	return location;
}
void barrel_TranscribeElement(BarrelService* service, BarrelNode* targetNode, void* target, unsigned int index, bool isRead)
{
	while (targetNode->_flags & req_ROLL ||
		targetNode->_flags & req_FREE) { /* Wait for lock to free */
	}

	InterlockedIncrement(&(targetNode->_userCount));

	void* barrelPtr = barrel_GetElementLocation(service, targetNode, index);
	void* readPtr = isRead ? barrelPtr : target;
	void* writePtr = isRead ? target : barrelPtr;

	rawTranscribe(readPtr, writePtr, targetNode->_vector._type->_size);

	InterlockedDecrement(&(targetNode->_userCount));
}
void barrel_TranscribeSpan(CollectionRequest request)
{
	bool isRead = request._type & TRANSCRIBE_COLLECTION_TO_RAW;
	void* readPtr = isRead ? NULL : request._trg;
	void* writePtr = isRead ? request._trg : NULL;
	void** barrelPtr = isRead ? &readPtr : &writePtr;

	BarrelNode* targetNode = isRead ? request._src : request._trg;
	

	int currentElement = isRead ? request._srcIx : request._trgIx;
	int direction = request._count < 0 ? -1 : 1;
	int count = request._count * direction;

	int barrelsPerBlock = targetNode->_vector._type->_barrelsPerBlock;
	int unitsPerBlock = targetNode->_vector._type->_unitsPerBlock;
	int vectorCount = targetNode->_vector._count;

	size_t unitSize = targetNode->_vector._type->_size;
	size_t collectionSize = targetNode->_vector._capacity * sizeof(Barrel);
	size_t collectionHead = (targetNode->_barrelStart * sizeof(Barrel)) + request._size; // Substitute for the heapHead
	size_t collectionOffset = targetNode->_barrelOffset * sizeof(Barrel);

	while (targetNode->_flags & req_ROLL ||
		targetNode->_flags & req_FREE) { /* Wait for lock to free */
	}

	InterlockedIncrement(&(targetNode->_userCount));

	while (count) {

		currentElement = currentElement >= vectorCount ? 0 : currentElement < 0 ? vectorCount - 1 : currentElement;

		*barrelPtr = ((collectionOffset + (currentElement * unitSize)) % collectionSize) + collectionHead;

		if (currentElement % unitsPerBlock ||				// Is not currently started on a 'memory bound' edge
			vectorCount - currentElement < unitsPerBlock ||	// Remaining elements not enough to fill a 'block'
			count < unitsPerBlock)							// Total elements left to write less than one 'block'
		{
			rawTranscribe(readPtr, writePtr, unitSize);

			writePtr = (size_t)writePtr + unitSize;

			currentElement += direction;
			count--;
		}
		else												// Move entire barrels at a time safely
		{
			barrelTranscribe(readPtr, writePtr, barrelsPerBlock);

			writePtr = (size_t)writePtr + (sizeof(Barrel) * barrelsPerBlock);
			
			currentElement += (direction * unitsPerBlock);
			count -= unitsPerBlock;
		}
	}

	InterlockedDecrement(&(targetNode->_userCount));

	return true;
}


bool barrel_ReadFromVector(CollectionRequest request) {

	BarrelNode* node = request._src;

	if (!(request._count))
		return true;

	if (request._srcIx + request._count >= node->_vector._count &&
		!(node->_vector._type->_flags & FLEXIBLE))
		return false;
	
	barrel_TranscribeSpan(request);

	return true;
}

bool barrel_WriteToVector(CollectionRequest request)
{
	BarrelNode* node = request._trg;
	BarrelService* service = request._service;

	if (node->_vector._type->_flags & READ_ONLY)
		return false;

	if (!(request._count))
		return true;

	if (request._trgIx + request._count >= node->_vector._count)
	{
		if (!(node->_vector._type->_flags & FLEXIBLE))
			return false;

		CollectionRequest resize = {node, NULL, 0,0,}

		if(!barrel_ResizeRequest())
			return false;
	}

	barrel_TranscribeSpan(request);

	return true;
}




bool barrel_Iterate(CollectionRequest* request)
{
	unsigned int* current = &(request->_trgIx);
	unsigned int* count = &(request->_count);

	if (!barrel_ReadFromVector(*request))
		return false;

	*current += *count;
	*count = *count - 1;

	return true;
}

bool barrel_Transcribe(CollectionRequest request)
{
	switch (request._type)
	{
	case TRANSCRIBE_RAW_TO_COLLECTION:
		return barrel_WriteToVector(request);

	case TRANSCRIBE_COLLECTION_TO_RAW:
		return barrel_ReadFromVector(request);

	case TRANSCRIBE_COLLECTIONS:
		return barrel_WriteToVector(request);


	default:
		return false;
	}
}

bool barrel_Modify(CollectionRequest request)
{
	switch (request._type)
	{
	case MODIFY_INSERT:
		return Bucket_Insert(request);

	default:
		return false;
	}
}

void barrelRoll(BarrelService* service, int index, int delta, int dir)
{
	// dir: -1 = left 1 = right

	if (dir == 0 || !service)
	{
		PREENT("Roll init failed!\n");
		return;
	}
		
	Barrel* _heapHead = (Barrel*)(service->_heap->_heapStart);
	BarrelNode* node = barrel_NodeLocation(service, index);

	int absDelta = delta < 0 ? delta * -1 : delta;

	if (node->_vector._capacity > 0)
	{
		int last = node->_barrelStart + (node->_vector._capacity - 1);

		// full roll

		if (delta == 0)
		{
			if (dir > 0)
			{
				_heapHead[last + 1] = _heapHead[node->_barrelStart];
				node->_barrelOffset -= node->_barrelOffset % node->_vector._type->_barrelsPerBlock ? 0 : 1;
				node->_barrelStart++;
			}

			else
			{
				_heapHead[node->_barrelStart - 1] = _heapHead[last];
				node->_barrelOffset += node->_barrelOffset % node->_vector._type->_barrelsPerBlock ? 0 : 1;
				node->_barrelStart--;
			}
		}

		// with no offset, the end is guaranteed, no action needed

		//   _ ||||||||||||| _

		else
		{
			// tail end rolling

			if ((delta < 0 && dir < 0) ||
				(delta > 0 && dir > 0))
			{
				if (node->_barrelOffset > 0)

					for (int j = 0; j < absDelta; j++)
					{
						if (dir > 0) // roll forward    ||||||| -> |||||||
							for (int i = last; i >= node->_barrelStart + node->_barrelOffset; i--)
								_heapHead[i + 1] = _heapHead[i];

						else // roll backward           ||||||| <- |||||||
							for (int i = node->_barrelStart + (node->_barrelOffset - 1); i < node->_barrelStart + (node->_vector._capacity - 1); i++)
								_heapHead[i] = _heapHead[i + 1];
					}
			}

			// head end rolling

			else
			{
				for (int j = 0; j < absDelta; j++)
				{
					if (dir > 0) // roll forward    ||||||| -> |||||||
					{
						if (node->_barrelOffset > 0)
							for (int i = node->_barrelStart + (node->_barrelOffset - 2); i >= node->_barrelStart; i--)
								_heapHead[i + 1] = _heapHead[i];

						node->_barrelOffset--;
						node->_barrelStart++;
					}

					else // roll backward           ||||||| <- |||||||
					{
						if (node->_barrelOffset > 0)
							for (int i = node->_barrelStart; i < node->_barrelStart + (node->_barrelOffset - 1); i++)
								_heapHead[i] = _heapHead[i + 1];

						node->_barrelOffset++;
						node->_barrelStart--;
					}
				}
			}
		}
	}

	else if (delta == 0 ||
		(delta < 0 && dir > 0) ||
		(delta > 0 && dir < 0))
		node->_barrelStart += dir > 0 ? 1 : -1;

	node->_vector._capacity += delta;

	node->_requests -= delta;

	node->_vector._capacity = node->_vector._capacity < 0 ? 0 : node->_vector._capacity;

	node->_barrelOffset =
		node->_barrelOffset < 0 ?
		node->_vector._capacity > node->_barrelOffset * -1 ? (node->_barrelOffset + node->_vector._capacity) % node->_vector._capacity : 0 :
		node->_barrelOffset > node->_vector._capacity ?
		node->_vector._capacity > 0 ? node->_barrelOffset % node->_vector._capacity : 0 :
		node->_barrelOffset;
}

DWORD WINAPI barrelRollingWork(void* target)
{
	if (!target)
		return 1;

	ThreadHandle* myThreadHandle = target;
	BarrelService* service = myThreadHandle->_service;

	if (!service || !(service->_heap))
		return 1;

	BarrelNode* workNode = NULL;
	BarrelNode* nextNode = NULL;

	int _index = myThreadHandle->_offset;

	while (service->_barrelNodes._localFlags & RUN)
	{
		_index = _index >= service->Omegus._vector._count ? myThreadHandle->_offset - 1 : _index;

		workNode = barrel_NodeLocation(service, _index);
		nextNode = barrel_NodeLocation(service, workNode->_nextNode);

		int delta = workNode->_requests < 0 ? -1 : workNode->_requests > 0 ? 1 : 0;

		if (_index == service->_lastPhysicalNode &&
			service->_barrelNodes._localFlags & req_INIT)
		{
			if (nextNode)
			{
				nextNode->_barrelStart = workNode->_barrelStart + workNode->_vector._capacity;
				service->_lastPhysicalNode = workNode->_nextNode;
			}

			service->_barrelNodes._localFlags &= ~req_INIT;
		}

		else if (workNode->_flags & req_ROLL)
		{
			while (workNode->_userCount) { /* Call might hang... */ }

			// take this opportunity to shrink the barrel in place by 1 and allow the immediate roll forward
			if (workNode->_requests < 0)
			{
				barrelRoll(service, _index, -1, 1);
				workNode->_flags &= ~req_ROLL;
			}

			// check for last physical index and see if page space is available
			else if (!nextNode)
			{
				int availableBarrels = barrel_maximumAvailableBarrels(service, workNode->_requests);
				barrelRoll(service, _index, availableBarrels, 1);
				workNode->_flags &= ~req_ROLL;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(nextNode->_flags & wait_ROLL))
			{
				nextNode->_flags |= req_ROLL;
				workNode->_flags |= wait_ROLL;
			}

			else if (workNode->_flags & wait_ROLL &&
				!(workNode->_flags & req_ROLL))
			{
				barrelRoll(service, _index, delta, 1);
				workNode->_flags &= ~wait_ROLL;
				workNode->_flags &= ~req_ROLL;
			}
		}

		else if (workNode->_flags & req_FREE)
		{
			while (workNode->_userCount) { /* Call might hang... */ }

			// take this opportunity to grow the barrel in place by 1 and allow the immediate roll backward
			if (workNode->_requests > 0)
			{
				barrelRoll(service, _index, 1, -1);
				workNode->_flags &= ~req_FREE;
			}

			// check for last index for automatic rolling rights
			else if (!nextNode)
			{
				barrelRoll(service, _index, workNode->_requests, -1);
				workNode->_flags &= ~req_FREE;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(nextNode->_flags & req_FREE))
			{
				barrelRoll(service, _index, delta, -1);
				workNode->_flags &= workNode->_requests != 0 ? 0xFFFFFFFF : ~req_FREE;
			}
		}

		else if (workNode->_requests != 0)
		{
			workNode->_flags |= workNode->_requests < 0 ? req_FREE : req_ROLL;
		}

		_index += service->_threadCount;
	}

	if (workNode)
	{
		//PREENT_ARGS("Thread % Terminating!\n", fmt_i(nextNode->_index));
		workNode->_flags = 0;
	}

	else
	{
		PREENT("Null Node");
	}
		
	//node->_tHandle = NULL;

	return 0;
}
DWORD WINAPI barrelServiceWork(void* target)
{
	if (!target)
		return 1;

	ThreadHandle* myThreadHandle = (ThreadHandle*)target;
	BarrelService* service = (BarrelService*)(myThreadHandle->_service);

	if (!service)
		return 1;

	BarrelNode* heapStart = (BarrelNode*)(service->_heap->_heapStart);

	if (!heapStart)
		return 1;

	while (service->_barrelNodes._localFlags & RUN)
	{
		int threshHold = service->_threadCount + 1;
		threshHold *= threshHold;
		threshHold -= 1;
		if (threshHold < service->Omegus._vector._count + 1)
		{
			ThreadHandle_ctor(&(service->_threadBin[service->_threadCount]), service, barrelRollingWork, service->_threadCount);
			service->_threadCount++;
		}

		QueRequest barrelRequest;

		if (RollingQue_PullRequest(&(service->_barrelNodes._que), &barrelRequest))
		{
			int nextNode = barrel_NextAvailableNode(service);
			*((int*)(barrelRequest._target)) = nextNode;
		}

	
	}

	return 0;
}

void BarrelNode_ctor(BarrelService* service, BarrelNode* barrel, Vector vector)
{
	barrel->_vector._count = 0;
	barrel->_vector._capacity = 0;
	barrel->_vector._type = vector._type;

	barrel->_flags = RUN;
	barrel->_nextNode = NONE;
	barrel->_barrelStart = NONE;

	barrel->_barrelOffset = 0;
	barrel->_userCount = 0;
	barrel->_requests = 0;
}
void BarrelServiceInit(BarrelService* barrelService, HeapService* heapService, ThreadHandle* threadBin, TypeID* barrelNodeTypeID)
{
	barrelService->_lastPhysicalNode = OMEGA;
	barrelService->_nextAvailable = NONE;
	barrelService->_threadBin = threadBin;
	barrelService->_threadCount = 0;
	barrelService->_heap = heapService;

	BarrelNode_ctor(
		barrelService,
		&(barrelService->Omegus),
		(Vector) {barrelNodeTypeID, 0});

	barrelService->Omegus._barrelStart = 0;

	tService_ctor(barrelService, barrelServiceWork);

	GlobalBarrelService = barrelService;
}

bool barrel_RequestNode(BarrelService* barrelService, BarrelNode** nodeLoc, Vector vector)
{
	int nodeIndex = -1; // Set index and wait for que to provide a target node
	int* testPtr = &nodeIndex;
	if (!RollingQue_MakeRequest(&(barrelService->_barrelNodes._que), (QueRequest) { 1, &nodeIndex }))
		return false;

	while (nodeIndex < 0) { /* Call might hang... */ }
	PREENT("Node index recieved!\n");

	// Initialize the requestedNode once one has been retrieved
	BarrelNode* requestedNode = barrel_NodeLocation(barrelService, nodeIndex);
	BarrelNode_ctor(barrelService, requestedNode, vector);

	// Set the last physical nodes "next node" to the newly created node, but not the service, and flip the init flag.
	BarrelNode* lastPhysicalNode = barrel_LastPhysicalNode(barrelService);
	if (!lastPhysicalNode)
		return false;

	// Set the next physical node for the current one, and request a node initialization with the flag/
	lastPhysicalNode->_nextNode = nodeIndex;
	barrelService->_barrelNodes._localFlags |= req_INIT;

	// Wait for the old 'lastPhysicalNode' to initialize the new one
	while(barrelService->_barrelNodes._localFlags & req_INIT) { /* Call might hang... */ }

	// Send the vector resize request with the initial vector elements
	if (!barrel_VectorResizeRequest(barrelService, requestedNode, vector._count, vector._bucket))
		return false;

	// Return the new BarrelNode to the user
	*nodeLoc = requestedNode;
	return true;
}

bool barrel_RemoveNode(BarrelService* barrelService, BarrelNode* node)
{
	return false;
}

