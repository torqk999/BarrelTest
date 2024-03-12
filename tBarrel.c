#include <tBarrel.h>

//const QueRequest NULL_REQUEST = { 0,NULL };

BarrelService GlobalBarrelService;
TypeID GlobalBarrelNodeType =
{
	sizeof(BarrelNode),
	"BarrelNode",
	BARREL,
	NULL
};

ThreadHandle barrelService_ThreadBin[MaxThreadCount];
Request barrelService_QueBin[MaxQueCount];

BarrelNode* barrel_NodeLocation(int index)
{
	return
		index == OMEGA ? &(GlobalBarrelService.Omegus) :
		index <= NONE || index >= GlobalBarrelService.Omegus._collection._count ? NULL :
		&((BarrelNode*)(GlobalBarrelService._heap->_heapStart))[index];
}
BarrelNode* barrel_LastPhysicalNode() { return barrel_NodeLocation(GlobalBarrelService._lastPhysicalNode); }

void* barrel_GetElementLocation(BarrelNode* node, uint index)
{
	ullong byteLength = node->_collection._capacity * sizeof(Barrel);
	ullong byteOffset = node->_barrelOffset * sizeof(Barrel);
	ullong byteIndex = index * node->_collection._type->_size;

	void* location = (void*)
		(((byteOffset + byteIndex) % byteLength) +
			(node->_barrelStart * sizeof(Barrel)) +
			(ullong)GlobalBarrelService._heap->_heapStart);

	return location;
}
void barrel_TranscribeElement(BarrelNode* targetNode, void* target, unsigned int index, bool isRead)
{
	while (targetNode->_flags & req_ROLL ||
		targetNode->_flags & req_FREE) { /* Wait for lock to free */
	}

	InterlockedIncrement(&(targetNode->_userCount));

	void* barrelPtr = barrel_GetElementLocation(targetNode, index);
	void* readPtr = isRead ? barrelPtr : target;
	void* writePtr = isRead ? target : barrelPtr;

	rawTranscribe(readPtr, writePtr, targetNode->_collection._type->_size);

	InterlockedDecrement(&(targetNode->_userCount));
}
void barrel_TranscribeSpan(Request request)
{
	bool isRead = request._type & TRANSCRIBE_COLLECTION_TO_RAW;
	void* readPtr = isRead ? NULL : request._trg;
	void* writePtr = isRead ? request._trg : NULL;
	void** barrelPtr = isRead ? &readPtr : &writePtr;

	BarrelNode* targetNode = isRead ? (BarrelNode*)request._src : (BarrelNode*)request._trg;


	int currentElement = isRead ? request._srcIx : request._trgIx;
	int direction = request._count < 0 ? -1 : 1;
	int count = request._count * direction;

	int barrelsPerBlock = targetNode->_collection._type->_barrelsPerBlock;
	int unitsPerBlock = targetNode->_collection._type->_unitsPerBlock;
	int vectorCount = targetNode->_collection._count;

	size_t unitSize = targetNode->_collection._type->_size;
	size_t collectionSize = targetNode->_collection._capacity * sizeof(Barrel);
	size_t collectionHead = (targetNode->_barrelStart * sizeof(Barrel)) + request._size; // Substitute for the heapHead
	size_t collectionOffset = targetNode->_barrelOffset * sizeof(Barrel);

	while (targetNode->_flags & req_ROLL ||
		targetNode->_flags & req_FREE) { /* Wait for lock to free */
	}

	InterlockedIncrement(&(targetNode->_userCount));

	while (count) {

		currentElement = currentElement >= vectorCount ? 0 : currentElement < 0 ? vectorCount - 1 : currentElement;

		*barrelPtr = (Barrel*)(((collectionOffset + (currentElement * unitSize)) % collectionSize) + collectionHead);

		if (currentElement % unitsPerBlock ||				// Is not currently started on a 'memory bound' edge
			vectorCount - currentElement < unitsPerBlock ||	// Remaining elements not enough to fill a 'block'
			count < unitsPerBlock)							// Total elements left to write less than one 'block'
		{
			rawTranscribe(readPtr, writePtr, unitSize);

			writePtr = (void*)((size_t)writePtr + unitSize);

			currentElement += direction;
			count--;
		}
		else												// Move entire barrels at a time safely
		{
			barrelTranscribe(readPtr, writePtr, barrelsPerBlock);

			writePtr = (void*)((size_t)writePtr + (sizeof(Barrel) * barrelsPerBlock));

			currentElement += (direction * unitsPerBlock);
			count -= unitsPerBlock;
		}
	}

	InterlockedDecrement(&(targetNode->_userCount));

}

size_t barrel_VectorRemainingSizeCap(BarrelNode* node)
{
	return (node->_collection._capacity * sizeof(Barrel)) - (node->_collection._count * node->_collection._type->_size);
}
uint barrel_VectorRemainingUnitCap(BarrelNode* node)
{
	return barrel_VectorRemainingSizeCap(node) / node->_collection._type->_size;
}

bool barrel_DeltaSizeRequest(BarrelNode* node, int delta)
{
	if (delta == 0)
		return true;

	int deltaMag = delta < 0 ? -delta : delta;

	size_t remainingCapacity = barrel_VectorRemainingSizeCap(node);
	size_t deltaSize = deltaMag * node->_collection._type->_size;

	// determine initial block 'delta', rounding up if positive, to cover any trailing elements
	int requestDelta =
		delta > 0 ? ((deltaSize - remainingCapacity) / sizeof(Barrel)) + ((deltaSize - remainingCapacity) % sizeof(Barrel) > 0 ? 1 : 0) :
		delta < 0 ? ((remainingCapacity - deltaSize) / sizeof(Barrel)) : 0;

	// bound to a full block
	while (requestDelta % node->_collection._type->_barrelsPerBlock)
		requestDelta += requestDelta > 0 ? 1 : -1;



	// set request and wait for it to process
	node->_requests += requestDelta;
	while (node->_requests) { /* Call might hang... */ }

	node->_collection._count += delta;

	return true;
}
bool barrel_ResizeRequest(Request request)
{
	BarrelNode* node = (BarrelNode*)request._trg;
	return barrel_DeltaSizeRequest(node, request._count - node->_collection._count);
}

int barrel_NextAvailableNode()
{

	int next = GlobalBarrelService._nextAvailable;

	if (next < 0)
	{
		PREENT("Getting Fresh Node...\n");
		next = GlobalBarrelService.Omegus._collection._count;
		barrel_DeltaSizeRequest(&(GlobalBarrelService.Omegus), 1, NULL);
		PREENT("Fresh Node!\n");
	}
	else
	{
		BarrelNode* nextNode = barrel_NodeLocation(next);
		GlobalBarrelService._nextAvailable = nextNode->_nextNode;
		PREENT("Recycled Node!\n");
	}

	return next;
}
int barrel_maximumAvailableBarrels(int requested)
{
	int availableBarrels = heap_Remaining(GlobalBarrelService._heap) / sizeof(Barrel);
	return availableBarrels < requested ? availableBarrels : requested;
}

bool barrel_Read(Request request) {

	BarrelNode* node = (BarrelNode*)request._src;

	if (!(request._count))
		return true;

	uint newCount = request._trgIx + request._count;

	if (newCount >= node->_collection._count)
	{
		if (!(node->_collection._type->_flags & FLEXIBLE))
			return false;

		Request resize = { WAIT_RESIZE, node, NULL, NULL, NULL, 0,0,newCount };

		if (!(node->_collection._type->_flags & ROLLING) &&
			!barrel_ResizeRequest(resize))
			return false;
	}

	barrel_TranscribeSpan(request);

	return true;
}
bool barrel_Write(Request request)
{
	BarrelNode* node = (BarrelNode*)request._trg;

	if (node->_collection._type->_flags & READ_ONLY)
		return false;

	if (!(request._count))
		return true;

	uint newCount = request._trgIx + request._count;

	if (newCount >= node->_collection._count)
	{
		if (!(node->_collection._type->_flags & FLEXIBLE))
			return false;

		Request resize = { WAIT_RESIZE, node, NULL, NULL, NULL, 0,0,newCount };

		if (!(node->_collection._type->_flags & ROLLING) &&
			!barrel_ResizeRequest(resize))
			return false;
	}

	barrel_TranscribeSpan(request);

	return true;
}
bool barrel_Insert(Request request)
{
	Bucket* trg = request._trg;

	if (trg->_collection._count >= trg->_collection._capacity)
	{
		trg->_collection._count = trg->_collection._capacity;
		return false;
	}

	void* insertion = request._src;
	int target = request._trgIx;

	request._src = trg->_bucket;
	request._srcIx = trg->_collection._count - 1;
	request._trgIx = trg->_collection._count + request._count - 1;
	request._count *= -1;

	barrel_Write(request);

	request._src = insertion;
	request._srcIx = 0;
	request._trgIx = target;
	request._count *= 1;

	barrel_Write(request);

	trg->_collection._count++;
	return true;
}
bool barrel_Remove(Request request) {}
bool barrel_RemoveAt(Request request) {}

bool barrel_Iterate(Request* request)
{
	unsigned int* current = &(request->_trgIx);
	unsigned int* count = &(request->_count);

	if (!barrel_Read(*request))
		return false;

	*current += *count;
	*count = *count - 1;

	return true;
}
bool barrel_Transcribe(Request request)
{
	switch (request._type)
	{
	case TRANSCRIBE_RAW_TO_COLLECTION:
		return barrel_Write(request);

	case TRANSCRIBE_COLLECTION_TO_RAW:
		return barrel_Read(request);

	case TRANSCRIBE_COLLECTIONS:
		return barrel_Write(request);


	default:
		return false;
	}
}
bool barrel_Modify(Request request)
{
	switch (request._type)
	{
	case MODIFY_INSERT:
		return barrel_Insert(request);

	case MODIFY_REMOVE_AT:
		return barrel_RemoveAt(request);

	case MODIFY_REMOVE_FIRST_FOUND:
		return barrel_Remove(request);

	case MODIFY_RESIZE:
		return barrel_ResizeRequest(request);

	default:
		return false;
	}
}

CollectionExtensions barrel_TemplateExtensions =
{
	barrel_Iterate,
	barrel_Transcribe,
	barrel_Modify
};

void barrelRoll(int index, int delta, int dir)
{
	// dir: -1 = left 1 = right

	if (dir == 0)
	{
		PREENT("Roll init failed!\n");
		return;
	}

	Barrel* _heapHead = (Barrel*)(GlobalBarrelService._heap->_heapStart);
	BarrelNode* node = barrel_NodeLocation(index);

	int absDelta = delta < 0 ? delta * -1 : delta;

	if (node->_collection._capacity > 0)
	{
		int last = node->_barrelStart + (node->_collection._capacity - 1);

		// full roll

		if (delta == 0)
		{
			if (dir > 0)
			{
				_heapHead[last + 1] = _heapHead[node->_barrelStart];
				node->_barrelOffset -= node->_barrelOffset % node->_collection._type->_barrelsPerBlock ? 0 : 1;
				node->_barrelStart++;
			}

			else
			{
				_heapHead[node->_barrelStart - 1] = _heapHead[last];
				node->_barrelOffset += node->_barrelOffset % node->_collection._type->_barrelsPerBlock ? 0 : 1;
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
							for (int i = node->_barrelStart + (node->_barrelOffset - 1); i < node->_barrelStart + (node->_collection._capacity - 1); i++)
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

	node->_collection._capacity += delta;

	node->_requests -= delta;

	node->_collection._capacity = node->_collection._capacity < 0 ? 0 : node->_collection._capacity;

	node->_barrelOffset =
		node->_barrelOffset < 0 ?
		node->_collection._capacity > node->_barrelOffset * -1 ? (node->_barrelOffset + node->_collection._capacity) % node->_collection._capacity : 0 :
		node->_barrelOffset > node->_collection._capacity ?
		node->_collection._capacity > 0 ? node->_barrelOffset % node->_collection._capacity : 0 :
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

	//QueRequest currentRequest = NULL_REQUEST;


	int _index = myThreadHandle->_offset;

	while (service->_barrelNodes._localFlags & RUN)
	{
		_index = _index >= service->Omegus._collection._count ? OMEGA : _index;

		workNode = barrel_NodeLocation(_index);
		nextNode = barrel_NodeLocation(workNode->_nextNode);

		int delta = workNode->_requests < 0 ? -1 : workNode->_requests > 0 ? 1 : 0;

		if (_index == service->_lastPhysicalNode &&
			service->_barrelNodes._localFlags & req_INIT)
		{
			if (nextNode)
			{
				nextNode->_barrelStart = workNode->_barrelStart + workNode->_collection._capacity;
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
				barrelRoll(_index, -1, 1);
				workNode->_flags &= ~req_ROLL;
			}

			// check for last physical index and see if page space is available
			else if (!nextNode)
			{
				int availableBarrels = barrel_maximumAvailableBarrels(workNode->_requests);
				barrelRoll(_index, availableBarrels, 1);
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
				barrelRoll(_index, delta, 1);
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
				barrelRoll(_index, 1, -1);
				workNode->_flags &= ~req_FREE;
			}

			// check for last index for automatic rolling rights
			else if (!nextNode)
			{
				barrelRoll(_index, workNode->_requests, -1);
				workNode->_flags &= ~req_FREE;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(nextNode->_flags & req_FREE))
			{
				barrelRoll(_index, delta, -1);
				workNode->_flags &= workNode->_requests != 0 ? 0xFFFFFFFF : ~req_FREE;
			}
		}

		else if (workNode->_requests != 0)
		{
			workNode->_flags |= workNode->_requests < 0 ? req_FREE : req_ROLL;
		}

	Next:
		_index += barrel_NodeCount();
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
		int threshHold = service->_threadBin._collection._count + 1;
		threshHold *= threshHold;
		threshHold -= 1;
		if (threshHold < service->Omegus._collection._count + 1)
		{
			int newIndex = service->_threadBin._collection._count;
			service->_threadBin._collection._count++;
			ThreadHandle_ctor(Bucket_GetPtr(&(service->_threadBin), newIndex), service, barrelRollingWork, newIndex);
		}

		Request barrelRequest;


		
		if (RollingQue_PullNext(&(service->_requests), &barrelRequest))
		{
			int nextNode = barrel_NextAvailableNode();
			*((int*)(barrelRequest._trg)) = nextNode;
		}
	}

	return 0;
}

BarrelNode BarrelNode_ctor(TypeID* type) {

	BarrelNode node;

	node._collection = Collection_ctor(type, &barrel_TemplateExtensions, 0, 0);

	node._nextNode = NONE;
	node._barrelStart = NONE;
	node._barrelOffset = 0;
	node._flags = 0;
	node._pointerCount = 0;
	node._userCount = 0;
	node._requests = 0;

	return node;
}

bool BarrelServiceInit(HeapService* heapService)
{
	if (!heapService)
	{
		PREENT("HeapService failed to load!\n");
		GlobalBarrelService._barrelNodes._localFlags = 0;
		return false;
	}
	
	GlobalBarrelService._heap = heapService;

	GlobalBarrelService.Omegus = BarrelNode_ctor(&GlobalBarrelNodeType);

	GlobalBarrelService._threadBin = Bucket_ctor(
		Collection_ctor(
			&ThreadHandle_TypeID,
			&Bucket_TemplateExtensions,
			MaxThreadCount, MaxThreadCount),
		barrelService_ThreadBin);

	GlobalBarrelService._requests = RollingQue_ctor(
		Bucket_ctor(
			Collection_ctor(
				&Request_TypeID,
				&Bucket_TemplateExtensions,
				MaxQueCount, MaxQueCount),
			barrelService_QueBin)
	);

	GlobalBarrelService._nextAvailable = NONE;
	GlobalBarrelService._lastPhysicalNode = OMEGA;

	return Service_Start(&GlobalBarrelService, barrelServiceWork);
}

bool barrel_RequestNode(BarrelNode** nodeLoc, Bucket src)
{
	int nodeIndex = -1; // Set index and wait for que to provide a target node

	Request nodeRequest = { CREATE_NODE, &nodeIndex };

	while (!RollingQue_Append(&(GlobalBarrelService._requests), &nodeRequest)) { /* Call might hang... */ }

	while (nodeIndex < 0) { /* Call might hang... */ }
	PREENT("Node index recieved!\n");

	// Initialize the requestedNode once one has been retrieved
	BarrelNode* requestedNode = barrel_NodeLocation(nodeIndex);
	*requestedNode = BarrelNode_ctor(src._collection._type);

	// Set the last physical nodes "next node" to the newly created node, but not the service, and flip the init flag.
	BarrelNode* lastPhysicalNode = barrel_LastPhysicalNode();
	if (!lastPhysicalNode)
		return false;

	// Set the next physical node for the current one, and request a node initialization with the flag/
	lastPhysicalNode->_nextNode = nodeIndex;
	GlobalBarrelService._barrelNodes._localFlags |= req_INIT;

	// Wait for the old 'lastPhysicalNode' to initialize the new one
	while (GlobalBarrelService._barrelNodes._localFlags & req_INIT) { /* Call might hang... */ }

	Request resizeRequest = {
		MODIFY_RESIZE,
		requestedNode,
		src._bucket,
		NULL,
		0, 0,
		src._collection._count};

	// Send the vector resize request with the initial vector elements
	if (!barrel_ResizeRequest(resizeRequest))
		return false;

	// Return the new BarrelNode to the user
	*nodeLoc = requestedNode;
	return true;
}
bool barrel_RemoveNode(BarrelNode* node)
{
	if (!barrel_ResizeRequest((Request) { MODIFY_RESIZE, node, NULL, NULL, 0, 0, 0 }))
		return false;

	BarrelNode* oldNextAvailable = barrel_NodeLocation(GlobalBarrelService._nextAvailable);


	return true;
}

uint barrel_NodeCount() { return GlobalBarrelService.Omegus._collection._count; }