#pragma once
#include <tServices.h>
#include <Windows.h>

bool heapDeltaPages(HeapService* heapService, int delta);

size_t heap_Remaining(HeapService* heap);

DWORD WINAPI heapServiceWork(void* target);

void ClearPage(HeapService* service, int index);

bool HeapServiceInit(HeapService* heapService, bool clear);