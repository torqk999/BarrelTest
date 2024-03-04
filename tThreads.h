#pragma once
#include <tHelpers.h>

bool ThreadHandle_ctor(ThreadHandle* threadHandle, void* service, LPTHREAD_START_ROUTINE workEntry, int offset);
