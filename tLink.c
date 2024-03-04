#include <tLink.h>

void LinkElements(Link* target, Link* prev, Link* next)
{
	if (target)
	{
		target->_next = next;
		target->_prev = prev;
	}

	if (next)
		next->_prev = target;

	if (prev)
		prev->_next = target;
}

void UnLink(Link* target)
{
	if (target->_next)
		target->_next->_prev = target->_prev;

	if (target->_prev)
		target->_prev->_next = target->_next;
}