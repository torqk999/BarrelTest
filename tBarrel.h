#pragma once
#include <tHeap.h>

BarrelNode* barrel_NodeLocation(BarrelService* service, int index)
{
	return  (BarrelNode*)&((Barrel*)(service->_heap->_heapStart))[index * BarrelsPerNode];
}
int barrel_NextAvailableNode(BarrelService* service)
{
	if (!service)
		return -1;

	int next = service->_nextAvailable;

	if (next < 0)
	{
		next = service->Omegus._vector._count;
		service->Omegus._requests++;
		while (service->Omegus._requests > 0) { /* Call might hang? */ }
	}
	else
	{
		BarrelNode* nextNode = barrel_NodeLocation(service, next);
		service->_nextAvailable = nextNode->_nextNode;
	}
	return next;
}
int barrel_NextAvailableBarrel(BarrelService* service)
{
	if (service->_lastPhysicalNode < 0)
		return 0;
	BarrelNode* lastNode = barrel_NodeLocation(service, service->_lastPhysicalNode);
	return lastNode->_barrelStart + lastNode->_barrelCount;
}
size_t barrel_VectorRemainingSizeCap(BarrelNode* node)
{
	return (node->_barrelCount * sizeof(Barrel)) - (node->_vector._count * node->_vector._type._size);
}
uint barrel_VectorRemainingUnitCap(BarrelNode* node)
{
	return barrel_VectorRemainingSizeCap(node) / node->_vector._type._size;
}
bool barrel_VectorResizeRequest(BarrelNode* node, int delta)
{
	size_t remainingCapacity = barrel_VectorRemainingSizeCap(node);
	size_t deltaSize = delta * node->_vector._type._size;

	int requestDelta =
		delta > 0 ? ((deltaSize - remainingCapacity) / sizeof(Barrel)) + ((deltaSize - remainingCapacity) % sizeof(Barrel) > 0 ? 1 : 0 ) :
		delta < 0 ? ((remainingCapacity - deltaSize) / sizeof(Barrel)) : 0 ;

	while (requestDelta % node->_barrelsPerBlock)
		requestDelta = requestDelta > 0 ? requestDelta++ : requestDelta--;

	node->_requests += requestDelta;
	return requestDelta != 0;
}
bool barrelNode_GetElementLocation(BarrelService* service, uint nodeIndex, uint index, void** target)
{

	BarrelNode* node = barrel_NodeLocation(service, nodeIndex);

	uint unitCapacity = (node->_barrelCount / node->_barrelsPerBlock) * node->_unitsPerBlock;

	if (index >= unitCapacity)
		return false;

	ullong byteLength = node->_barrelCount * sizeof(Barrel);
	ullong byteOffset = (node->_barrelOffset * node->_barrelsPerBlock) * sizeof(Barrel);
	ullong byteIndex = index * node->_vector._type._size;

	// vector offset + barrel offset + heap ptr

	*target = (void*)
		(((byteOffset + byteIndex) % byteLength) +
		(node->_barrelStart * sizeof(Barrel)) +
		(ullong)service->_heap->_heapStart);

	//Vector_MoveIndex(source, target, 0, 0, node->_vector._type._size);

	return true;
}
bool deltaBarrelNodes(BarrelService* barrelService, int delta, BarrelNode** newNode)
{
	if (!barrelService)
		return false;

	HeapService* heapService = barrelService->_heap;
	if (!heapService)
		return false;

	/*
* 1 - Check for available page space
* 2 - Check for available recycle node
* 3 - If not, request to extend node collection
* 4 - Assign node, initialize vector
* 5 - Assign adiitional threads if needed
*/

	while(RollingQue_HasRequests(&(heapService->_pages._que))) { /* idle... (Possible hang!) */ }

	if (delta < 0)
	{

	}

	if (delta > 0)
	{
		int locIndex = 0;

		while (delta > 0)
		{
			BarrelNode* newNode = barrel_NextAvailableNode(barrelService);
			if (!newNode)
			{
				barrelService->Omegus._requests += delta;

			}
		}
		

		//heapService->_heapEnd = ((size_t)heapService->_heapEnd) + (BarrelSize * BarrelsPerNode);

	}

	

	return true;
}
void barrelRoll(BarrelService* service, int index, int delta, int dir)
{
	// dir: -1 = left 1 = right

	if (dir == 0 || !service)
		return;

	Barrel* _heapHead = (Barrel*)(service->_heap->_heapStart);
	BarrelNode* node = (BarrelNode*) & (_heapHead[index * BarrelsPerNode]);

	if (!node)
		return;

	int absDelta = delta < 0 ? delta * -1 : delta;

	if (node->_barrelCount > 0)
	{
		int last = node->_barrelStart + (node->_barrelCount - 1);

		// full roll

		if (delta == 0)
		{
			if (dir > 0)
			{
				_heapHead[last + 1] = _heapHead[node->_barrelStart];
				node->_barrelOffset -= node->_barrelOffset % node->_barrelsPerBlock ? 0 : 1;
				node->_barrelStart++;
			}

			else
			{
				_heapHead[node->_barrelStart - 1] = _heapHead[last];
				node->_barrelOffset += node->_barrelOffset % node->_barrelsPerBlock ? 0 : 1;
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
							for (int i = node->_barrelStart + (node->_barrelOffset - 1); i < node->_barrelStart + (node->_barrelCount - 1); i++)
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

	node->_barrelCount += delta;

	node->_requests -= delta;

	node->_barrelCount = node->_barrelCount < 0 ? 0 : node->_barrelCount;

	node->_barrelOffset =
		node->_barrelOffset < 0 ?
		node->_barrelCount > node->_barrelOffset * -1 ? (node->_barrelOffset + node->_barrelCount) % node->_barrelCount : 0 :
		node->_barrelOffset > node->_barrelCount ?
		node->_barrelCount > 0 ? node->_barrelOffset % node->_barrelCount : 0 :
		node->_barrelOffset;
}
int maximumAvailableBarrels(BarrelService* barrelService, int requested)
{
	int availableBarrels = heap_Remaining(barrelService->_heap) / sizeof(Barrel);
	return availableBarrels < requested ? availableBarrels : requested;
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

	int _index = myThreadHandle->_offset;

	while (service->_barrelNodes._localFlags & RUN)
	{
		_index = _index >= service->Omegus._vector._count ? myThreadHandle->_offset : _index;

		workNode = _index == 0 ? &(service->Omegus) : barrel_NodeLocation(service, _index - 1);

		int delta = workNode->_requests < 0 ? -1 : workNode->_requests > 0 ? 1 : 0;

		if (workNode->_flags & req_ROLL)
		{
			// take this opportunity to shrink the barrel in place by 1 and allow the immediate roll forward
			if (workNode->_requests < 0)
			{
				barrelRoll(service, _index, -1, 1);
				workNode->_flags &= ~req_ROLL;
			}

			// check for last index and see if page space is available
			else if (!workNode)
			{
				int availableBarrels = maximumAvailableBarrels(service, workNode->_requests);
				barrelRoll(service, _index, availableBarrels, 1);
				workNode->_flags &= ~req_ROLL;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(workNode->_flags & wait_ROLL))
			{
				workNode->_flags |= req_ROLL;
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
			// take this opportunity to grow the barrel in place by 1 and allow the immediate roll backward
			if (workNode->_requests > 0)
			{
				barrelRoll(service, _index, 1, -1);
				workNode->_flags &= ~req_FREE;
			}

			// check for last index for automatic rolling rights
			else if (!workNode)
			{
				barrelRoll(service, _index, workNode->_requests, -1);
				workNode->_flags &= ~req_FREE;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(workNode->_flags & req_FREE))
			{
				barrelRoll(service, _index, delta, -1);
				workNode->_flags |= req_FREE;
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
			int nextNodeIndex = barrel_NextAvailableNode(service);
			*((int*)(barrelRequest._target)) = nextNodeIndex;
			service->_lastPhysicalNode = nextNodeIndex;
		}
			
	}

	return 0;
}

int barrel_NextAdjacentIndex(BarrelService* service, int targetStartBarrel)
{
	int nextIndex = -1;

	if (targetStartBarrel < 0)
		return nextIndex;

	int nextStartBarrel = targetStartBarrel * 2;

	for (int i = 0; i < service->Omegus._vector._count; i++)
	{
		BarrelNode* nextNode = barrel_NodeLocation(service, i);
		if (nextNode->_barrelStart == targetStartBarrel)
			continue;

		int nextPossible = nextNode->_barrelStart;
		if (nextPossible > targetStartBarrel && nextPossible < nextStartBarrel)
		{
			nextStartBarrel = nextPossible;
			nextIndex = i;
		}
	}
	return nextIndex;
}

void BarrelNode_ctor(
	BarrelService* service,
	BarrelNode* barrel,
	Vector vector)
{
	barrel->_vector = vector;
	barrel->_barrelStart = barrel_NextAvailableBarrel(service);
	barrel->_nextNode = barrel_NextAdjacentIndex(service, barrel->_barrelStart);

	// initialized barrel is aligned and has no requests, locks, or garbage
	barrel->_barrelOffset = 0;
	barrel->_requests = 0;
	barrel->_locked = 0;

	// block is the smallest contiguous chunk of memory bounded units of both barrels and vector units
	// set for vector unit size equals sizeof Barrel

	int unitsPerBlock = 1;
	int barrelsPerBlock = 1;

	// check less or greater conditions, adjust units per 'block' accordingly
	
	if (vector._type._size < sizeof(Barrel))
	{
		while ((barrelsPerBlock * sizeof(Barrel)) % vector._type._size)
			barrelsPerBlock++;
		
		unitsPerBlock = (barrelsPerBlock * sizeof(Barrel)) / vector._type._size;
	}

	else if (vector._type._size > sizeof(Barrel))
	{
		while ((unitsPerBlock * vector._type._size) % sizeof(Barrel))
			unitsPerBlock++;

		barrelsPerBlock = (unitsPerBlock * vector._type._size) / sizeof(Barrel);
	}

	barrel->_barrelCount =  
		((vector._count / unitsPerBlock) + (vector._count % unitsPerBlock > 0)) * barrelsPerBlock;

	barrel->_unitsPerBlock = unitsPerBlock;
	barrel->_barrelsPerBlock = barrelsPerBlock;

	barrel->_flags = 0;
}

void BarrelServiceInit(BarrelService* barrelService, HeapService* heapService, ThreadHandle* threadBin) // Omegus Barrel Vector   >:-|
{
	barrelService->_nextAvailable = -1;
	barrelService->_threadBin = threadBin;
	barrelService->_threadCount = 0;
	barrelService->_heap = heapService;

	BarrelNode_ctor(
		barrelService,
		&(barrelService->Omegus),
		(Vector) {TYPE_ID(BarrelNode, barrel_NodeLocation(barrelService, 0)), 0});

	tService_ctor(barrelService, barrelServiceWork);
}

bool barrel_RequestNode(BarrelService* barrelService, BarrelNode** nodeLoc, Vector vector)
{
	int nodeIndex = -1;
	if (!RollingQue_MakeRequest(&(barrelService->_barrelNodes._que), (QueRequest) { 1, & nodeIndex }))
		return false;

	while (nodeIndex < 0) {}
	*nodeLoc = barrel_NodeLocation(barrelService, nodeIndex);

	BarrelNode_ctor(
		barrelService,
		*nodeLoc,
		vector);
	return true;
}

bool barrel_RemoveNode(BarrelService* barrelService, BarrelNode* node)
{

}