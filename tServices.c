#include <tServices.h>

void tService_ctor(tService* service, LPTHREAD_START_ROUTINE serviceThreadStart)
{
	service->_localFlags = RUN;

	RollingQue_ctor(&(service->_que));
	ThreadHandle_ctor(&(service->_serviceThread), service, serviceThreadStart, 0);
}