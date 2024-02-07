#pragma once
#include <tModules.h>

void RollingQue_Clear(RollingQue* que, int index)
{
	que->_requests[index]._flag = NULL;
	que->_requests[index]._delta = 0;
}

void RollingQue_ClearAll(RollingQue* que)
{
	for (int i = 0; i < QUE_SIZE; i++)
		RollingQue_Clear(que, i);
}

void RollingQue_ctor(RollingQue* que)
{
	que->_end = 0;
	que->_start = 0;
	RollingQue_ClearAll(que);
}

void Advance(int* index)
{
	*index = *index++;
	if (*index >= QUE_SIZE)
		*index = 0;
}

bool RollingQue_MakeRequest(RollingQue* que, QueRequest request)
{
	int check = que->_end;
	Advance(&check);
	if (check == que->_start)
		return false;

	que->_requests[que->_end] = request;
	que->_end = check;
	return true;
}

bool RollingQue_PullRequest(RollingQue* que, QueRequest* request)
{
	if (que->_start == que->_end)
		return false;
	
	*request = que->_requests[que->_start];
	Advance(&que->_start);
	return true;
}

bool RollingQue_HasRequests(RollingQue* que)
{
	return que->_start != que->_end;
}