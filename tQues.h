#ifndef	tQues
#define	tQues
#include <tBucket.h>

//TYPE_DECL(Request, NULL, 0);
const TypeInfo Request_TypeID;

void RollingQue_Clear(RollingQue* que, int index);

void RollingQue_ClearAll(RollingQue* que);

RollingQue RollingQue_ctor(Bucket bucket);

ULONG Advance(volatile ULONG* index);

bool RollingQue_Append(RollingQue* que, void* input);

bool RollingQue_PullNext(RollingQue* que, void* output);

bool RollingQue_HasQue(RollingQue* que);

bool RollingQue_SeeNext(RollingQue* que, void* trg);
#endif