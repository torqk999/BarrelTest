#include <tLink.h>

void LinkElements(Link* target, Link* prev, Link* next)
{
	if (target)
	{
		while (target->_lock) {}
		InterlockedIncrement(&(target->_lock));
		target->_next = next;
		target->_prev = prev;
		InterlockedDecrement(&(target->_lock));
	}

	if (next)
	{
		while (next->_lock) {}
		InterlockedIncrement(&(next->_lock));
		next->_prev = target ? target : prev;
		InterlockedDecrement(&(next->_lock));
	}
		
	if (prev)
	{
		while (prev->_lock){}
		InterlockedIncrement(&(prev->_lock));
		prev->_next = target ? target : next;
		InterlockedDecrement(&(prev->_lock));
	}
}

void UnLink(Link* target)
{
	LinkElements(NULL, target->_prev, target->_next);
}