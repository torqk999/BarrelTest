#ifndef	tHeap
#define	tHeap
#include <tServices.h>

bool heapDeltaPages(int delta);

size_t heap_Remaining();

DWORD WINAPI heapServiceWork(void* target);

void ClearPage(int index);

HeapService* HeapServiceInit(bool clear);

#endif