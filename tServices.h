#ifndef	tServices
#define	tServices

#include <tThreads.h>
#include <tQues.h>

//tService tService_ctor(RollingQue serviceQue);

bool Service_Start(tService* service, LPTHREAD_START_ROUTINE serviceThreadStart);
#endif