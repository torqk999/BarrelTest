#include <tBarrel.h>
//
////const QueRequest NULL_REQUEST = { 0,NULL };
//
//BarrelService GlobalBarrelService;
//TypeInfo GlobalBarrelNodeType =
//{
//	sizeof(BarrelNode),
//	"BarrelNode",
//	BARREL,
//	NULL
//};
//
//ThreadHandle barrelService_ThreadBin[MaxThreadCount];
//Request barrelService_QueBin[MaxQueCount];
//
//
//bool barrel_Read(Request request) {
//
//	BarrelNode* node = (BarrelNode*)request._src;
//
//	if (!(request._size))
//		return true;
//
//	uint newCount = request._trgIx + request._size;
//
//	if (newCount >= node->_managed._collection._count)
//	{
//		if (node->_managed._collection._info->_flags & FIXED_SIZE)
//			return false;
//
//		//Request resize = { WAIT_RESIZE, node, NULL, NULL, NULL, 0,0,newCount };
//
//		if (!barrel_ResizeRequest(ResizeCollection(node, NULL, newCount)))
//			return false;
//	}
//
//	barrel_TranscribeSpan(request);
//
//	return true;
//}
//bool barrel_Write(Request request)
//{
//	BarrelNode* node = (BarrelNode*)request._trg;
//	void* src = request._src;
//
//	if (node->_managed._collection._info->_flags & READ_ONLY)
//		return false;
//
//	if (!(request._size))
//		return true;
//
//	uint newCount = request._trgIx + request._size;
//
//	if (newCount >= node->_managed._collection._count)
//	{
//		if (node->_managed._collection._info->_flags & FIXED_SIZE)
//			return false;
//
//		//Request resize = { MODIFY_DELTA_CAPACITY, node, NULL, NULL, NULL, 0,0,newCount };
//
//		if (!barrel_Resize(ResizeCollection(node, src, newCount)))
//			return false;
//	}
//
//	barrel_TranscribeSpan(request);
//
//	return true;
//}
//bool barrel_Insert(Request request)
//{
//	Bucket* trg = request._trg;
//
//	if (trg->_collection._count >= trg->_collection._capacity)
//	{
//		trg->_collection._count = trg->_collection._capacity;
//		return false;
//	}
//
//	void* insertion = request._src;
//	int target = request._trgIx;
//
//	request._src = trg->_bucket;
//	request._srcIx = trg->_collection._count - 1;
//	request._trgIx = trg->_collection._count + request._size - 1;
//	request._size *= -1;
//
//	barrel_Write(request);
//
//	request._src = insertion;
//	request._srcIx = 0;
//	request._trgIx = target;
//	request._size *= 1;
//
//	barrel_Write(request);
//
//	trg->_collection._count++;
//	return true;
//}
//bool barrel_Remove(Request request) {}
//bool barrel_RemoveAt(Request request) {}
//
//bool barrel_Iterate(Request* request)
//{
//	unsigned int* current = &(request->_trgIx);
//	unsigned int* count = &(request->_size);
//
//	if (!barrel_Read(*request))
//		return false;
//
//	*current += *count;
//	*count = *count - 1;
//
//	return true;
//}
//
//void barrel_AppendRequest(LinkedRequest* linkedRequest) {
//	Request request = linkedRequest->_request;
//	BarrelNode* node = (BarrelNode*)(request._trg);
//	if (!(node->_requests))
//	{
//		node->_requests = linkedRequest;
//	}
//	else
//	{
//		LinkElements(linkedRequest, node->_requests, NULL);
//	}
//}
//
//
//void barrelRoll(int index, int delta, int dir)
//{
//	// dir: -1 = left 1 = right
//
//	if (dir == 0)
//	{
//		PREENT("Roll init failed!\n");
//		return;
//	}
//
//	Barrel* _heapHead = (Barrel*)(GlobalBarrelService._heap->_heapStart);
//	BarrelNode* node = barrel_NodeLocation(index);
//
//	int absDelta = delta < 0 ? delta * -1 : delta;
//
//	if (node->_managed._collection._capacity > 0)
//	{
//		int last = node->_barrelStart + (node->_managed._collection._capacity - 1);
//
//		// full roll
//
//		if (delta == 0)
//		{
//			if (dir > 0)
//			{
//				_heapHead[last + 1] = _heapHead[node->_barrelStart];
//				node->_barrelOffset -= node->_barrelOffset % node->_managed._collection._info->_barrelsPerBlock ? 0 : 1;
//				node->_barrelStart++;
//			}
//
//			else
//			{
//				_heapHead[node->_barrelStart - 1] = _heapHead[last];
//				node->_barrelOffset += node->_barrelOffset % node->_managed._collection._info->_barrelsPerBlock ? 0 : 1;
//				node->_barrelStart--;
//			}
//		}
//
//		// with no offset, the end is guaranteed, no action needed
//
//		//   _ ||||||||||||| _
//
//		else
//		{
//			// tail end rolling
//
//			if ((delta < 0 && dir < 0) ||
//				(delta > 0 && dir > 0))
//			{
//				if (node->_barrelOffset > 0)
//
//					for (int j = 0; j < absDelta; j++)
//					{
//						if (dir > 0) // roll forward    ||||||| -> |||||||
//							for (int i = last; i >= node->_barrelStart + node->_barrelOffset; i--)
//								_heapHead[i + 1] = _heapHead[i];
//
//						else // roll backward           ||||||| <- |||||||
//							for (int i = node->_barrelStart + (node->_barrelOffset - 1); i < node->_barrelStart + (node->_managed._collection._capacity - 1); i++)
//								_heapHead[i] = _heapHead[i + 1];
//					}
//			}
//
//			// head end rolling
//
//			else
//			{
//				for (int j = 0; j < absDelta; j++)
//				{
//					if (dir > 0) // roll forward    ||||||| -> |||||||
//					{
//						if (node->_barrelOffset > 0)
//							for (int i = node->_barrelStart + (node->_barrelOffset - 2); i >= node->_barrelStart; i--)
//								_heapHead[i + 1] = _heapHead[i];
//
//						node->_barrelOffset--;
//						node->_barrelStart++;
//					}
//
//					else // roll backward           ||||||| <- |||||||
//					{
//						if (node->_barrelOffset > 0)
//							for (int i = node->_barrelStart; i < node->_barrelStart + (node->_barrelOffset - 1); i++)
//								_heapHead[i] = _heapHead[i + 1];
//
//						node->_barrelOffset++;
//						node->_barrelStart--;
//					}
//				}
//			}
//		}
//	}
//
//	else if (delta == 0 ||
//		(delta < 0 && dir > 0) ||
//		(delta > 0 && dir < 0))
//		node->_barrelStart += dir > 0 ? 1 : -1;
//
//	node->_managed._collection._capacity += delta;
//
//	//node->_requests -= delta;
//
//	node->_managed._collection._capacity = node->_managed._collection._capacity < 0 ? 0 : node->_managed._collection._capacity;
//
//	node->_barrelOffset =
//		node->_barrelOffset < 0 ?
//		node->_managed._collection._capacity > node->_barrelOffset * -1 ? (node->_barrelOffset + node->_managed._collection._capacity) % node->_managed._collection._capacity : 0 :
//		node->_barrelOffset > node->_managed._collection._capacity ?
//		node->_managed._collection._capacity > 0 ? node->_barrelOffset % node->_managed._collection._capacity : 0 :
//		node->_barrelOffset;
//}
//void barrelInit(){}
//
//DWORD WINAPI barrelRollingWork(void* target)
//{
//	if (!target)
//		return 1;
//
//	ThreadHandle* myThreadHandle = target;
//	BarrelService* service = myThreadHandle->_service;
//
//	if (!service || !(service->_heap))
//		return 1;
//
//	BarrelNode* workNode = NULL;
//	BarrelNode* nextNode = NULL;
//
//	//QueRequest currentRequest = NULL_REQUEST;
//
//
//	int _index = myThreadHandle->_offset;
//
//	while (service->_barrelNodes._localFlags & RUN)
//	{
//		_index = _index >= service->Omegus._managed._collection._count ? OMEGA : _index;
//
//		workNode = barrel_NodeLocation(_index);
//		nextNode = barrel_NodeLocation(workNode->_nextNode);
//
//		int barrelRequestCount = workNode->_requests ? workNode->_requests->_request._size : 0;
//		int delta = barrelRequestCount < 0 ? -1 : barrelRequestCount > 0 ? 1 : 0;
//
//		//if (_index == service->_lastPhysicalNode &&
//		//	service->_barrelNodes._localFlags & req_INIT)
//		//{
//		//	if (nextNode)
//		//	{
//		//		workNode->_nextNode = nextNode;
//		//		nextNode->_barrelStart = workNode->_barrelStart + workNode->_collection._capacity;
//		//		service->_lastPhysicalNode = workNode->_nextNode;
//		//	}
//		//
//		//	service->_barrelNodes._localFlags &= ~req_INIT;
//		//}
//
//		if (workNode->_flags & req_ROLL)
//		{
//			while (workNode->_managed._userCount) { /* Call might hang... */ }
//
//			// take this opportunity to shrink the barrel in place by 1 and allow the immediate roll forward
//			if (barrelRequestCount < 0)
//			{
//				barrelRoll(_index, -1, 1);
//				barrelRequestCount -= 1;
//				workNode->_flags &= ~req_ROLL;
//			}
//
//			// check for last physical index and see if page space is available
//			else if (!nextNode)
//			{
//				int availableBarrels = barrel_maximumAvailableBarrels(barrelRequestCount);
//				barrelRequestCount = 0;
//				barrelRoll(_index, availableBarrels, 1);
//				workNode->_flags &= ~req_ROLL;
//			}
//
//			// inform the next barrel that there is a requested slot to roll to
//			else if (!(nextNode->_flags & wait_ROLL))
//			{
//				nextNode->_flags |= req_ROLL;
//				workNode->_flags |= wait_ROLL;
//			}
//
//			else if (workNode->_flags & wait_ROLL &&
//				!(workNode->_flags & req_ROLL))
//			{
//				barrelRoll(_index, delta, 1);
//				barrelRequestCount -= delta;
//				workNode->_flags &= ~wait_ROLL;
//				workNode->_flags &= ~req_ROLL;
//			}
//		}
//
//		else if (workNode->_flags & req_FREE)
//		{
//			while (workNode->_managed._userCount) { /* Call might hang... */ }
//
//			// take this opportunity to grow the barrel in place by 1 and allow the immediate roll backward
//			if (barrelRequestCount > 0)
//			{
//				barrelRoll(_index, 1, -1);
//				barrelRequestCount += 1;
//				workNode->_flags &= ~req_FREE;
//			}
//
//			// check for last index for automatic rolling rights
//			else if (!nextNode)
//			{
//				barrelRoll(_index, barrelRequestCount, -1);
//				barrelRequestCount = 0;
//				workNode->_flags &= ~req_FREE;
//			}
//
//			// inform the next barrel that there is a requested slot to roll to
//			else if (!(nextNode->_flags & req_FREE))
//			{
//				barrelRoll(_index, delta, -1);
//				barrelRequestCount -= delta;
//				workNode->_flags &= barrelRequestCount != 0 ? 0xFFFFFFFF : ~req_FREE;
//			}
//		}
//
//		else if (workNode->_requests)
//		{
//			switch (workNode->_requests->_request._type)
//			{
//			case CREATE_NODE_BARREL:
//				workNode->_nextNode = nextNode;
//				nextNode->_barrelStart = workNode->_barrelStart + workNode->_managed._collection._capacity;
//				service->_lastPhysicalNode = workNode->_nextNode;
//				workNode->_requests->_request._type = DONE;
//				workNode->_requests = RemoveAndNext(workNode->_requests);
//				break;
//
//			case MODIFY_DELTA_CAPACITY:
//				workNode->_flags |= barrelRequestCount < 0 ? req_FREE : req_ROLL;
//				break;
//			default:
//				break;
//			}
//		}
//
//	Next:
//		_index += barrel_NodeCount();
//		if (workNode->_requests)
//		{
//			if (barrelRequestCount) // Update Request Counter
//				workNode->_requests->_request._size = barrelRequestCount;
//			else  // Move to next request in linked list
//				workNode->_requests = RemoveAndNext(workNode->_requests);
//		}
//			
//	}
//
//	if (workNode)
//	{
//		//PREENT_ARGS("Thread % Terminating!\n", fmt_i(nextNode->_index));
//		workNode->_flags = 0;
//	}
//
//	else
//	{
//		PREENT("Null Node");
//	}
//
//	//node->_tHandle = NULL;
//
//	return 0;
//}
//DWORD WINAPI barrelServiceWork(void* target)
//{
//	if (!target)
//		return 1;
//
//	ThreadHandle* myThreadHandle = (ThreadHandle*)target;
//	BarrelService* service = (BarrelService*)(myThreadHandle->_service);
//
//	if (!service)
//		return 1;
//
//	BarrelNode* heapStart = (BarrelNode*)(service->_heap->_heapStart);
//
//	if (!heapStart)
//		return 1;
//
//	while (service->_barrelNodes._localFlags & RUN)
//	{
//		int threshHold = service->_threadBin._collection._count + 1;
//		threshHold *= threshHold;
//		threshHold -= 1;
//		if (threshHold < service->Omegus._managed._collection._count + 1)
//		{
//			int newIndex = service->_threadBin._collection._count;
//			service->_threadBin._collection._count++;
//			ThreadHandle_ctor(Bucket_GetPtr(&(service->_threadBin), newIndex), service, barrelRollingWork, newIndex);
//		}
//
//		//Request barrelRequest;
//		//
//		//
//		//
//		//if (RollingQue_PullNext(&(service->_requests), &barrelRequest))
//		//{
//		//	int nextNode = barrel_NextAvailableNode();
//		//	*((int*)(barrelRequest._trg)) = nextNode;
//		//}
//	}
//
//	return 0;
//}
//
//BarrelNode BarrelNode_ctor(Collection collection) {
//
//	BarrelNode node;
//
//	node._managed = Managed_ctor(collection);
//	node._managed._collection._extensions = barrel_Extensions;
//
//	node._nextNode = NONE;
//	node._barrelStart = NONE;
//	node._barrelOffset = 0;
//	node._flags = OFF;
//	node._requests = NULL;
//
//	return node;
//}
//

inline BarrelNode* Barrel_GetNode(int index)
{
	return
		index == OMEGA ? &(GlobalBarrelService.Omegus) :
		index <= NONE || index >= GlobalBarrelService.Omegus._collection._count ? NULL :
		&((BarrelNode*)(GlobalBarrelService._heap->_heapStart))[index];
}
inline BarrelNode* Barrel_GetLastPhysicalNode() { return Barrel_GetNode(GlobalBarrelService._lastPhysicalNode); }
inline void Barrel_SetLastPhysicalNode(int set) { GlobalBarrelService._lastPhysicalNode = set; }

inline size_t Barrel_GetSize(BarrelNode* nodePtr) {
	return nodePtr->_barrelCount * sizeof(Barrel);
}

inline void* Barrel_GetHead(BarrelNode* nodePtr) {
	return (void*)
		((nodePtr->_barrelStart * sizeof(Barrel)) +
		(ullong)GlobalBarrelService._heap->_heapStart);
}

inline void* Barrel_GetBarrelPtr(uint index) {
	return (size_t)(GlobalBarrelService._heap->_heapStart) + (index * sizeof(Barrel));
}

void* Barrel_GetUnitPtr(BarrelNode* nodePtr, uint index) {
	return (void*)(
		((nodePtr->_barrelStart * sizeof(Barrel)) + (ullong)GlobalBarrelService._heap->_heapStart) + // Head
		(((nodePtr->_collection._extensions->_type->_size * index) + (nodePtr->_barrelOffset * sizeof(Barrel))) % // Target
		(nodePtr->_barrelCount * sizeof(Barrel))) // Size
		);
}

uint Barrel_NodeCount() {
	return GlobalBarrelService.Omegus._barrelCount;
}

inline void* Barrel_GetElementPtr(BarrelNode* nodePtr, uint index)
{
	BarrelNode node = *nodePtr;

	return (void*)
		((((node._barrelOffset * sizeof(Barrel)) + (index * node._collection._extensions->_type->_size))
			% (node._barrelCount * sizeof(Barrel))) +
				(node._barrelStart * sizeof(Barrel)) +
				(ullong)GlobalBarrelService._heap->_heapStart);
}

inline size_t Barrel_RemainingSizeCap(BarrelNode* node)
{
	return (node->_barrelCount * sizeof(Barrel)) - (node->_collection._count * node->_collection._extensions->_type->_size);
}
inline uint Barrel_RemainingUnitCap(BarrelNode* node)
{
	return Barrel_RemainingSizeCap(node) / node->_collection._extensions->_type->_size;
}

uint Barrel_maximumAvailableBarrels(uint requested)
{
	uint availableBarrels = Heap_Remaining(GlobalBarrelService._heap) / sizeof(Barrel);
	return availableBarrels < requested ? availableBarrels : requested;
}

inline void Barrel_Offset(BarrelNode* node, int request) {
	node->_barrelOffset -= request;
	node->_barrelOffset =
		node->_barrelOffset < 1 ? node->_barrelCount :
		node->_barrelOffset > node->_barrelCount ? 1 :
		node->_barrelOffset;
}
inline void Barrel_RollBack(BarrelNode* node, int request) {
	node->_barrelStart += request;
	for (int i = 0; i < -request; i++)
		barrelTranscribe(
			Barrel_GetBarrelPtr(node->_barrelStart + i),
			Barrel_GetBarrelPtr(node->_barrelStart + node->_barrelCount + i),
			-request);

	Barrel_Offset(node, request);
}
inline void Barrel_RollForward(BarrelNode* node, int request) {
	for (int i = 0; i < request; i++)
		barrelTranscribe(
			Barrel_GetBarrelPtr(node->_barrelStart + node->_barrelCount + i),
			Barrel_GetBarrelPtr(node->_barrelStart + i),
			request);
	node->_barrelStart += request;

	Barrel_Offset(node, request);
}
inline void Barrel_Grow(BarrelNode* node, int request) {
	int newCount = (int)(node->_barrelCount) + request;
	int newOffset = node->_barrelOffset + request;

	if (node->_barrelCount != node->_barrelOffset) {

		for (int i = newCount; i > node->_barrelOffset; i--)
			barrelTranscribe(
				Barrel_GetBarrelPtr(i - 1),				// last barrel working backwards
				Barrel_GetBarrelPtr(i - (1 + request)),	// barrel request distance from the last
				1);
	}

	node->_barrelCount = newCount;
	node->_barrelOffset = newOffset;
}
inline void Barrel_Shrink(BarrelNode* node, int request) {
	int newCount = (int)(node->_barrelCount) + request;
	int newOffset = node->_barrelOffset + request;

	if (node->_barrelCount != node->_barrelOffset) {

		request = newOffset < 0 ? request - newOffset : request;
		newOffset = newOffset < 0 ? 0 : newOffset;

		for (int i = newOffset; i < newCount; i++)
			barrelTranscribe(
				Barrel_GetBarrelPtr(i),					// offset barrel working forwards
				Barrel_GetBarrelPtr(i - request),		// barrel request distance from the offset
				1);

		newOffset = newOffset < 1 ? newCount : newOffset;
	}

	node->_barrelCount = newCount;
	node->_barrelOffset = newOffset;
}

// Returns whether the "Barrel-Roll" completed successfully
bool Barrel_Roll(BarrelNode* node, int request) {

	if (request == 0) // Empty, return good anyway
		return true;

	if (!Collection_Request(node)) // Failed to reserve collection
		return false;

	if (request < 0) // Shrink collection before anything
		Barrel_RollBack(node, request);
	
	if (node->_nextNode > -1) // Send action to next physical node
		return Barrel_Roll(Barrel_GetNode(node->_nextNode), request);

	else if (request > 0 && Heap_Remaining() < request * sizeof(Barrel)) // Heap has no available space for last node
		return false;

	if (request > 0) // Grow the collection last
		Barrel_RollForward(node, request);

	Collection_Release(node); // Release collection
	return true;
}
bool Barrel_DeltaSize(REQUEST request)
{
	int unitDelta = (int)(request._params[tCOUNT]);

	PREENT_ARGS("DeltaSize Request: %\n", fmt_i(unitDelta));
	//int countDelta = (int)(request._params[tCOUNT]);
	BarrelNode* node = request._params[tTRG];
	size_t unitSize = node->_collection._extensions->_type->_size;
	
	// no action needed
	if (unitDelta == 0)
		return true;

	int deltaMag = unitDelta < 0 ? -unitDelta : unitDelta;

	size_t remainingCapacity = Barrel_RemainingSizeCap(node);
	size_t deltaSize = deltaMag * node->_collection._extensions->_type->_size;

	// determine initial block 'delta', rounding up if positive, to cover any trailing elements
	int barrelDelta =
		unitDelta > 0 ? ((deltaSize - remainingCapacity) / sizeof(Barrel)) + ((deltaSize - remainingCapacity) % sizeof(Barrel) > 0 ? 1 : 0) :
		unitDelta < 0 ? (-(int)(remainingCapacity - deltaSize) / sizeof(Barrel)) : 0;

	// no action needed
	if (barrelDelta == 0)
		return true;

	// shrink first
	if (barrelDelta < 0)
		Barrel_Shrink(node, barrelDelta);

	// roll necessary Barrels
	if (node->_nextNode > NONE && !Barrel_Roll(Barrel_GetNode(node->_nextNode), barrelDelta))
		return false;

	// grow after
	if (barrelDelta > 0) {
		if (node->_nextNode <= NONE && Heap_Remaining() < barrelDelta * sizeof(Barrel))
			return false;

		Barrel_Grow(node, barrelDelta);
	}

	return true;
}

// Sets the size of the Barrel.
// Parameters:
// [tTRG] = BarrelNode* targetNode
// [tSRC] = void* initializerSource (for increase)
// [tCOUNT] = uint newSize

bool Barrel_Resize(REQUEST request) {
	BarrelNode* node = request._params[tTRG];
	((uint)request._params[tCOUNT]) -= node->_collection._count;
	return Barrel_DeltaSize(request);
}

int Barrel_NextAvailableNodeIx()
{

	int next = GlobalBarrelService._nextAvailable;

	if (next < 0)
	{
		PREENT("Getting Fresh Node...\n");
		next = GlobalBarrelService.Omegus._collection._count;
		REQUEST deltaSize = Request(MANAGE, P_(tVARIANT, DELTA_SIZE), P_(tTRG, &(GlobalBarrelService.Omegus)), P_(tSIZE, 1), P_(tCOUNT, 1));
		if (!Barrel_DeltaSize(deltaSize))
		{
			PREENT("Omegus Vector resize failed!\n");
			return NONE;
		}
		PREENT("Fresh Node!\n");
	}
	else
	{
		BarrelNode* nextNode = Barrel_GetNode(next);
		GlobalBarrelService._nextAvailable = nextNode->_nextNode;
		PREENT("Recycled Node!\n");
	}

	return next;
}




// Get the corresponding chunk snapshot of the target Barrel. Note: request is not thread safe.
// Use in conjunction with Collection_Request(COLLECTION) and Collection_Free(COLLECTION).
bool Barrel_GetChunk(BarrelNode* node, Chunk* loc) {
	Chunk_ctor(loc, Barrel_GetHead(node), Barrel_GetSize(node));
	return true;
}

uint Barrel_Capacity(BarrelNode* node) {
	size_t unitSize = node->_collection._extensions->_type->_size;
	size_t memSize = node->_barrelCount * sizeof(Barrel);
	return memSize / unitSize;
}

inline bool Barrel_Manage(REQUEST request){
	ParamType var = (ParamType)request._params[tVARIANT];
	BarrelNode* barrelNode = request._params[tSRC];

	switch (var)
	{
	case tCHUNK:
		request._params[tSIZE] = barrelNode->_barrelCount * sizeof(Barrel);
		return Barrel_GetChunk(barrelNode, request._params[tTRG]);

	case tUSE:
		return Collection_Request(barrelNode);

	case tRELEASE:
		return Collection_Release(barrelNode);

	case DELTA_SIZE:
		return Barrel_DeltaSize(request);

	case tRESIZE:
		return Barrel_Resize(request);


	default:
		return false;
	}
}
inline bool Barrel_Transcribe(REQUEST request){
	return false;
}
inline bool Barrel_Info(REQUEST request){
	BarrelNode* barrelNode = request._params[tSRC];
	ParamType var = (ParamType)request._params[tVARIANT];
	void* output = request._params[tTRG];

	switch (var) {

	case tCOUNT:
		*((uint*)output) = barrelNode->_collection._count;
		return true;

	case tCAPACITY:
		*((uint*)output) = Barrel_Capacity(barrelNode);
		return true;

	case tNAME:
	case tSIZE:
		return TypeInfo_GetInfo(request);

	default:
		return false;
	}
}

bool Barrel_Extensions(REQUEST request) {
	switch (request._type)
	{

	case INFO:
		return Barrel_Info(request);

	case TRANSCRIBE:
		return Barrel_Transcribe(request);

	case MANAGE:
		return Barrel_Manage(request);

	default:
		return false;
	}
}



void Barrel_NodeCtor(TypeInfo* info, BarrelNode* loc, void* srcHead, int memFlags, uint initCapacity, uint startBarrel) {

	loc->_barrelStart = startBarrel;

	bool empty = memFlags & EMPTY;
	bool fill = memFlags & FILL;
	memFlags &= !(EMPTY);
	memFlags &= !(FILL);
	memFlags |= FIXED_SIZE;

	int newBarrelCount = 1;
	size_t neededSpace = info->_size * initCapacity;

	while (	(newBarrelCount * sizeof(Barrel) < neededSpace) &&
			(newBarrelCount * sizeof(Barrel) < Heap_Remaining()))
		newBarrelCount *= 2;

	loc->_barrelCount = empty ? 0 : newBarrelCount;

	size_t newSize = newBarrelCount * sizeof(Barrel);

	initCapacity = (neededSpace < newSize ? neededSpace : newSize) / info->_size;

	Collection collTmp = Collection_Create(Collection_GetExtensions(info, Barrel_Extensions, memFlags), initCapacity);

	rawTranscribe(&(loc->_collection), &collTmp, sizeof(Collection));

	loc->_barrelOffset = loc->_barrelCount;
	loc->_nextNode = NONE;

	void* barrelHead = Barrel_GetBarrelPtr(startBarrel);
	
	for (int i = 0; i < initCapacity; i++)
		rawTranscribe(&((char*)barrelHead)[i * info->_size], &((char*)srcHead)[i * info->_size], info->_size);

	//if (srcHead)
	//	Collection_WriteSpan(loc, srcHead, 0, initCapacity);
}

BarrelNode* Barrel_Sourced(TypeInfo* info, void* srcHead, int memFlags, uint initCapacity) {

	// Get the next available node (either a new node must be added to the Omegus vector, or a garbage node will get recycled.
	int newNodeIx = Barrel_NextAvailableNodeIx();
	if (newNodeIx < 0)
		return NULL;

	// Aquire the last physical node so that it can initialize our new one
	BarrelNode* lastPhysicalNode = Barrel_GetLastPhysicalNode();
	if (!lastPhysicalNode)
		return NULL;

	lastPhysicalNode->_nextNode = newNodeIx;

	// Increment the global collection's counter before retrieving the ptr
	GlobalBarrelService.Omegus._collection._count++;

	// Initialize the requestedNode once one has been retrieved
	BarrelNode* newNodePtr = Barrel_GetNode(newNodeIx);
	Barrel_NodeCtor(info, newNodePtr, srcHead, MANAGED | memFlags, initCapacity, lastPhysicalNode->_barrelStart + lastPhysicalNode->_barrelCount);

	// Set the last physical node
	Barrel_SetLastPhysicalNode(newNodeIx);

	PREENT_ARGS("BarrelCount: %\n", fmt_i(newNodePtr->_collection._count));

	// Return the new BarrelNode to the user
	return newNodePtr;
}

COLLECTION Barrel_ctor(const char* typeName, size_t unitSize, void* srcHead, int memFlags, uint initCapacity) {

	TypeInfo* info = TypeInfo_Get(typeName, unitSize);
	if (!info)
		return NULL;

	return Barrel_Sourced(info, srcHead, memFlags, initCapacity);
}

bool Barrel_ServiceInit(HeapService* heapService)
{
	if (!heapService)
	{
		PREENT("HeapService failed to load!\n");
		//GlobalBarrelService._barrelNodes._localFlags = 0;
		return false;
	}

	GlobalBarrelService._heap = heapService;

	Barrel_NodeCtor("BarrelNode", sizeof(BarrelNode),  &(GlobalBarrelService.Omegus), NULL , MANAGED | EMPTY, 0, 0);

	GlobalBarrelService._nextAvailable = NONE;
	GlobalBarrelService._lastPhysicalNode = OMEGA;

	return true;
}


//bool barrel_RemoveNode(BarrelNode* node)
//{
//	if (!barrel_Resize((Request) { MODIFY_DELTA_CAPACITY, node, NULL, NULL, 0, 0 }))
//		return false;
//
//	BarrelNode* oldNextAvailable = barrel_NodeLocation(GlobalBarrelService._nextAvailable);
//
//	return true;
//}
//