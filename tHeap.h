#pragma once
#include <tServices.h>

bool heapDeltaPages(HeapService* heapService, int delta);

size_t heap_Remaining(HeapService* heap);

DWORD WINAPI heapServiceWork(void* target);

void ClearPage(HeapService* service, int index);

HeapService* HeapServiceInit(bool clear);