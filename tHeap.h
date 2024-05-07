#ifndef	tHeap
#define	tHeap
#include <tServices.h>

static HeapService GlobalHeapService;

bool Heap_DeltaPages(int delta);

size_t Heap_Remaining();

void* Heap_Head();

DWORD WINAPI Heap_ServiceWork(void* target);

void Heap_ClearPage(int index);

HeapService* Heap_ServiceInit(bool clear);

#endif