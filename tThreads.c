#include <tThreads.h>

TYPE_DECL(ThreadHandle, NULL, 0);

bool ThreadHandle_ctor(ThreadHandle* threadHandle, void* service, LPTHREAD_START_ROUTINE workEntry, int offset)
{
	if (!threadHandle || !service || !workEntry)
		return false;

	threadHandle->_service = service;
	threadHandle->_offset = offset;
	threadHandle->_handle = CreateThread(NULL, 0, workEntry, threadHandle, 0, &(threadHandle->_ID));

	return (threadHandle->_handle) != NULL;
}
