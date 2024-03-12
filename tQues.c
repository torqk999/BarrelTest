#include <tQues.h>

TYPE_DECL(Request, NULL, 0);

void RollingQue_Clear(RollingQue* que, int index)
{
	Collection_Write(&(que->_bucket), NULL, index);
}

void RollingQue_ClearAll(RollingQue* que)
{
	for (int i = 0; i < MaxQueCount; i++)
		RollingQue_Clear(que, i);

	que->_next = 0;
	que->_start = 0;
}

RollingQue RollingQue_ctor(Bucket bucket)
{
	RollingQue newQue = { bucket };
	RollingQue_ClearAll(&newQue);
	return newQue;
}

ULONG Advance(volatile ULONG* index)
{
	return InterlockedExchange(index, *index + 1 >= MaxQueCount ? 0 : *index + 1);
}

bool RollingQue_Append(RollingQue* que, void* next)
{
	if (que->_next + 1 == que->_start) {
		PREENT("Que Capacity Full! No request space!\n");
		return false;
	}

	PREENT("Request made!\n");
	return Collection_Write(&(que->_bucket), next, Advance(&que->_next));
}

bool RollingQue_PullNext(RollingQue* que, void* trg)
{
	if (que->_start == que->_next) {
		//PREENT("End of que! No requests left to process!\n");
		return false;
	}

	PREENT("Request pulled!\n");
	return Collection_Read(&(que->_bucket), trg, Advance(&que->_start));
}

bool RollingQue_HasQue(RollingQue* que)
{
	return que->_start != que->_next;
}

bool RollingQue_SeeNext(RollingQue* que, void* trg)
{
	if (que->_start == que->_next) {
		//PREENT("End of que! No requests left to process!\n");
		return false;
	}

	return Collection_Read(&(que->_bucket), trg, que->_start);
}

