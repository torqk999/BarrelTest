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
//BarrelNode* barrel_NodeLocation(int index)
//{
//	return
//		index == OMEGA ? &(GlobalBarrelService.Omegus) :
//		index <= NONE || index >= GlobalBarrelService.Omegus._managed._collection._count ? NULL :
//		&((BarrelNode*)(GlobalBarrelService._heap->_heapStart))[index];
//}
//BarrelNode* barrel_LastPhysicalNode() { return barrel_NodeLocation(GlobalBarrelService._lastPhysicalNode); }
//
//void* barrel_GetPtr(BarrelNode* nodePtr, uint index)
//{
//	BarrelNode node = *nodePtr;
//	//ullong byteLength = node._managed._collection._capacity * sizeof(Barrel);
//	//ullong byteOffset = node._barrelOffset * sizeof(Barrel);
//	//ullong byteIndex = index * node._managed._collection._info->_type._size;
//	//
//	//void* location = (void*)
//	//	(((byteOffset + byteIndex) % byteLength) +
//	//		(node._barrelStart * sizeof(Barrel)) +
//	//		(ullong)GlobalBarrelService._heap->_heapStart);
//
//	//return location;
//
//	return (void*)
//		((((node._barrelOffset * sizeof(Barrel)) + (index * node._managed._collection._info->_type._size)) % (node._managed._collection._capacity * sizeof(Barrel))) +
//				(node._barrelStart * sizeof(Barrel)) +
//				(ullong)GlobalBarrelService._heap->_heapStart);
//}
//
//
//size_t barrel_RemainingSizeCap(BarrelNode* node)
//{
//	return (node->_managed._collection._capacity * sizeof(Barrel)) - (node->_managed._collection._count * node->_managed._collection._info->_type._size);
//}
//uint barrel_RemainingUnitCap(BarrelNode* node)
//{
//	return barrel_RemainingSizeCap(node) / node->_managed._collection._info->_type._size;
//}
//
//bool barrel_Use(Request* request) {
//	BarrelNode* targetNode = request->_trg;
//
//	while (targetNode->_flags & req_ROLL ||
//		targetNode->_flags & req_FREE) { /* Wait for lock to free */ }
//
//	Managed_Use(targetNode);
//
//	return true;
//}
//
//bool barrel_DeltaSize(Request request)
//{
//	int delta = request._size;
//	BarrelNode* node = request._trg;
//	void* src = request._src;
//
//	if (delta == 0)
//		return true;
//
//	int deltaMag = delta < 0 ? -delta : delta;
//
//	size_t remainingCapacity = barrel_RemainingSizeCap(node);
//	size_t deltaSize = deltaMag * node->_managed._collection._info->_type._size;
//
//	// determine initial block 'delta', rounding up if positive, to cover any trailing elements
//	int requestDelta =
//		delta > 0 ? ((deltaSize - remainingCapacity) / sizeof(Barrel)) + ((deltaSize - remainingCapacity) % sizeof(Barrel) > 0 ? 1 : 0) :
//		delta < 0 ? ((remainingCapacity - deltaSize) / sizeof(Barrel)) : 0;
//
//	// bound to a full block
//	while (requestDelta % node->_managed._collection._info->_barrelsPerBlock)
//		requestDelta += requestDelta > 0 ? 1 : -1;
//
//	LinkedRequest deltaSizeRequest = { (Link) { NULL, NULL }, DeltaSizeCapacity(node, src, requestDelta) };
//	barrel_AppendRequest(&deltaSizeRequest);
//
//	while (deltaSizeRequest._request._size) { /* Call might hang... */ }
//
//	node->_managed._collection._count += delta;
//
//	return true;
//}
//bool barrel_Resize(Request request)
//{
//	BarrelNode* node = (BarrelNode*)request._trg;
//	request._size -= node->_managed._collection._count;
//	return barrel_DeltaSize(request);
//}
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
//int barrel_NextAvailableNode()
//{
//
//	int next = GlobalBarrelService._nextAvailable;
//
//	if (next < 0)
//	{
//		PREENT("Getting Fresh Node...\n");
//		next = GlobalBarrelService.Omegus._managed._collection._count;
//		if (!barrel_DeltaSizeRequest(&(GlobalBarrelService.Omegus), 1))
//		{
//			PREENT("Omegus Vector resize failed!\n");
//			return false;
//		}
//		PREENT("Fresh Node!\n");
//	}
//	else
//	{
//		BarrelNode* nextNode = barrel_NodeLocation(next);
//		GlobalBarrelService._nextAvailable = nextNode->_nextNode;
//		PREENT("Recycled Node!\n");
//	}
//
//	return next;
//}
//int barrel_maximumAvailableBarrels(int requested)
//{
//	int availableBarrels = heap_Remaining(GlobalBarrelService._heap) / sizeof(Barrel);
//	return availableBarrels < requested ? availableBarrels : requested;
//}
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
//bool barrel_Extensions(Request* request) {
//	switch (request->_type)
//	{
//	case COLLECTION_USE:
//		return barrel_Use(request);
//
//	case MODIFY_DELTA_CAPACITY:
//		return barrel_Resize(*request);
//
//	case TRANSCRIBE_RAW_TO_COLLECTION:
//	case TRANSCRIBE_COLLECTIONS:
//		return barrel_Write(*request);
//
//	case TRANSCRIBE_COLLECTION_TO_RAW:
//		return barrel_Read(*request);
//
//	case MODIFY_INSERT:
//		return barrel_Insert(*request);
//
//	case MODIFY_REMOVE_FIRST_FOUND:
//		return barrel_Remove(*request);
//
//	case MODIFY_REMOVE_AT:
//		return barrel_RemoveAt(*request);
//
//	case LOCATION:
//		return barrel_Head(*request);
//
//	case ITERATE:
//		return barrel_Iterate(request);
//
//	default:
//		return false;
//	}
//}
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
//bool BarrelServiceInit(HeapService* heapService)
//{
//	if (!heapService)
//	{
//		PREENT("HeapService failed to load!\n");
//		GlobalBarrelService._barrelNodes._localFlags = 0;
//		return false;
//	}
//	
//	GlobalBarrelService._heap = heapService;
//
//	GlobalBarrelService.Omegus = BarrelNode_ctor(Collection_ctor(&GlobalBarrelNodeType, 0, 0));
//
//	GlobalBarrelService._threadBin = Bucket_ctor(
//		Collection_ctor(
//			&ThreadHandle_TypeID,
//			MaxThreadCount, MaxThreadCount),
//		barrelService_ThreadBin);
//
//	GlobalBarrelService._nextAvailable = NONE;
//	GlobalBarrelService._lastPhysicalNode = OMEGA;
//
//	return Service_Start(&GlobalBarrelService, barrelServiceWork);
//}
//
//COLLECTION barrel_Construct(size_t size, const char* typeName, void* src)
//{
//	Bucket srcBucket = *((Bucket*)src); 
//	//void* srcLoc = request._src;
//
//	// Get the next available node (either a new node must be added to the Omegus vector,
//	// or a garbage node will get recycled.
//	int nodeIndex = barrel_NextAvailableNode();
//
//	// Initialize the requestedNode once one has been retrieved
//	BarrelNode* requestedNode = barrel_NodeLocation(nodeIndex);
//	*requestedNode = BarrelNode_ctor(Collection_ctor0(Type_CreateRaw(size, typeName), BARREL, barrel_Extensions, 0));
//
//	// Aquire the last physical node so that it can initialize our new one
//	BarrelNode* lastPhysicalNode = barrel_LastPhysicalNode();
//	if (!lastPhysicalNode)
//		return NULL;
//
//	// Put a linked request on stack, which will flag the successful nod init
//	// by the barrelThread Service
//	LinkedRequest nodeRequest = Linked(InitBarrelNode(requestedNode));
//	barrel_AppendRequest(&nodeRequest);
//
//	while (nodeRequest._request._type != DONE) { /* Call might hang... */ }
//
//	// Write into the new vector
//	Collection_WriteSpan(requestedNode, srcBucket._bucket, 0, srcBucket._collection._count);
//
//	// Return the new BarrelNode to the user
//	return requestedNode;
//}
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
//uint barrel_NodeCount() { return GlobalBarrelService.Omegus._managed._collection._count; }