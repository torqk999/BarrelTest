#include <tServices.h>

bool Service_Start(tService* service, LPTHREAD_START_ROUTINE serviceThreadStart) {
	service->_localFlags = RUN;
	return ThreadHandle_ctor(&(service->_serviceThread), service, serviceThreadStart, 0);
}