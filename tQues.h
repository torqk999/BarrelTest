#pragma once
#include <tHelpers.h>

void RollingQue_Clear(RollingQue* que, int index);

void RollingQue_ClearAll(RollingQue* que);

void RollingQue_ctor(RollingQue* que);

void Advance(int* index);

bool RollingQue_MakeRequest(RollingQue* que, QueRequest request);

bool RollingQue_PullRequest(RollingQue* que, QueRequest* request);

bool RollingQue_HasRequests(RollingQue* que);