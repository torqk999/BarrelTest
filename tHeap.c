#include <tHeap.h>

static HeapService GlobalHeapService;
static TypeInfo GlobalPageType;

bool Heap_DeltaPages(int delta)
{
	int theDelta = delta;
	delta = delta < 0 && (unsigned int)(delta * -1) > GlobalHeapService._pageCount ? -1 * (GlobalHeapService._pageCount) : delta; // auto-clamp

	if (delta == 0)
		return false; // no action taken

	if (delta > 0)
	{
		HeapAlloc(GlobalHeapService._heapStart, 0, delta * PageSize);
	}

	GlobalHeapService._pageCount += delta;

	if (delta < 0)
	{
		HeapFree(GlobalHeapService._heapStart, 0, (void*)((size_t)GlobalHeapService._heapStart + (GlobalHeapService._pageCount * (size_t)PageSize)));
	}
}

size_t Heap_Remaining()
{
	return ((size_t)(GlobalHeapService._heapStart) + (GlobalHeapService._pageCount * sizeof(Page))) - (size_t)(GlobalHeapService._heapEnd);
}

void* Heap_Head() { return GlobalHeapService._heapStart; }

DWORD WINAPI Heap_ServiceWork(void* target)
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

		if (!Heap_DeltaPages(pageDelta))
			break;
	}

	return 0;
}

void Heap_ClearPage(int index)
{
	Page* targetPage = &(((Page*)GlobalHeapService._heapStart)[index]);

	for (int i = 0; i < 512; i++)
		targetPage->_mem[i] = 0;
}

HeapService* Heap_ServiceInit(bool clear)
{
	GlobalHeapService._heapStart = HeapCreate(0, 0, 0);

	if (!GlobalHeapService._heapStart)
	{
		PREENT("Heap Create Failed!\n");
		return NULL;
	}
		

	ULONG ulEnableLFH = 2;

	if (!HeapSetInformation(GlobalHeapService._heapStart, HeapCompatibilityInformation, &ulEnableLFH, sizeof(ulEnableLFH)))
	{
		PREENT("Information Set Failed!\n");
		return NULL;
	}

	GlobalHeapService._heapEnd = GlobalHeapService._heapStart;
	GlobalHeapService._pageCount = 0;

	// Initialize service
	Service_Start(&(GlobalHeapService._pages), Heap_ServiceWork);

	// Add first Page
	Heap_DeltaPages(1);

	// Initialize the mapSpace to zeros
	if (clear)
		Heap_ClearPage(0);

	return &GlobalHeapService;
}