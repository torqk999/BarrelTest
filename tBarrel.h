#pragma once
#include <tHeap.h>

BarrelService GlobalBarrelService;
TypeID GlobalBarrelNodeType;

ThreadHandle barrelService_ThreadBin[MaxThreadCount];
Request barrelService_QueBin[MaxQueCount];

BarrelNode* barrel_NodeLocation(int index);
BarrelNode* barrel_LastPhysicalNode();

void* barrel_GetElementLocation(BarrelNode* node, uint index);
void barrel_TranscribeElement(BarrelNode* targetNode, void* target, unsigned int index, bool isRead);
void barrel_TranscribeSpan(Request request);

size_t barrel_VectorRemainingSizeCap(BarrelNode* node);
uint barrel_VectorRemainingUnitCap(BarrelNode* node);

bool barrel_DeltaSizeRequest(BarrelNode* node, int delta);

int barrel_NextAvailableNode();
int barrel_maximumAvailableBarrels(int requested);

bool barrel_Read(Request request);
bool barrel_Write(Request request);
bool barrel_Insert(Request request);
bool barrel_Remove(Request request);
bool barrel_RemoveAt(Request request);

bool barrel_Iterate(Request* request);
bool barrel_Transcribe(Request request);
bool barrel_ResizeRequest(Request request);
bool barrel_Modify(Request request);

void barrelRoll(int index, int delta, int dir);

DWORD WINAPI barrelRollingWork(void* target);
DWORD WINAPI barrelServiceWork(void* target);

BarrelNode BarrelNode_ctor(TypeID* type);
bool BarrelServiceInit(HeapService* heapService);

bool barrel_RequestNode(BarrelNode** nodeLoc, Bucket vector);
bool barrel_RemoveNode(BarrelNode* node);

uint barrel_NodeCount();