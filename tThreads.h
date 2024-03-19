#ifndef	tThreads
#define	tThreads
#include <tTypeInfo.h>

const TypeInfo ThreadHandle_TypeID;

bool ThreadHandle_ctor(ThreadHandle* threadHandle, void* service, LPTHREAD_START_ROUTINE workEntry, int offset);

#endif