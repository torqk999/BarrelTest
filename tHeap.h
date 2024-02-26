#pragma once
#include <tServices.h>
#include <Windows.h>

bool heapDeltaPages(HeapService* heapService, int delta)
{
	delta = delta < 0 && (unsigned int)(delta * -1) > heapService->_pageCount ? -1 * (heapService->_pageCount) : delta; // auto-clamp

	if (delta == 0)
		return false; // no action taken

	if (delta > 0)
	{
		HeapAlloc(heapService->_heapStart, 0, delta * PageSize);
	}

	heapService->_pageCount += delta;

	if (delta < 0)
	{
		HeapFree(heapService->_heapStart, 0, (void*)((size_t)heapService->_heapStart + (heapService->_pageCount * (size_t)PageSize)));
	}
} 

size_t heap_Remaining(HeapService* heap)
{
	return ((size_t)(heap->_heapStart) + (heap->_pageCount * sizeof(Page))) - (size_t)(heap->_heapEnd);
}

DWORD WINAPI heapServiceWork(void* target)
{
	if (!target)
		return 1;

	ThreadHandle* myThreadHandle = (ThreadHandle*)target;
	HeapService* service = (HeapService*)(myThreadHandle->_service);

	if (!service)
		return 1;

	Page* heapStart = (Page*)(service->_heapStart);

	if (!heapStart)
		return 1;

	while (service->_pages._localFlags & RUN)
	{
		size_t target = (size_t)(service->_heapEnd);
		size_t current = (size_t)(service->_heapStart) + (sizeof(Page) * service->_pageCount);

		int pageDelta = ((target > current ? target - current : current - target) / sizeof(Page)) - 1;

		if (!heapDeltaPages(service, pageDelta))
			break;
	}

	return 0;
}

void ClearPage(HeapService* service, int index)
{
	Barrel* pageStart = (Barrel*) &(((Page*)(service->_heapStart))[index]);

	for (int i = 0; i < 128; i++)
		pageStart[i] = (Barrel){ 0,0,0,0 };
}

bool HeapServiceInit(HeapService* heapService, bool clear)
{
	heapService->_heapStart = HeapCreate(0, 0, 0);
	
	int myStackVar;

	if (!heapService->_heapStart)
		return false;

	ULONG ulEnableLFH = 2;

	if (!HeapSetInformation(heapService->_heapStart, HeapCompatibilityInformation, &ulEnableLFH, sizeof(ulEnableLFH)))
	{
		PREENT("Information Set Failed!\n");
		return false;
	}

	heapService->_heapEnd = heapService->_heapStart;
	heapService->_pageCount = 0;

	// Initialize service
	tService_ctor(&(heapService->_pages), heapServiceWork);

	// Add first Page
	heapDeltaPages(heapService, 1);

	// Initialize the mapSpace to zeros
	if (clear)
		ClearPage(heapService, 0);

	return true;
}