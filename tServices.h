#pragma once
#include <tThreads.h>
#include <tQues.h>

void tService_ctor(tService* service, LPTHREAD_START_ROUTINE serviceThreadStart);