#pragma once
#include <tHeap.h>
#include <tTypeID.h>

static BarrelService* GlobalBarrelService;
static TypeID* GlobalBarrelNodeType;

BarrelNode* barrel_NodeLocation(BarrelService* service, int index);
BarrelNode* barrel_LastPhysicalNode(BarrelService* service);

void* barrel_GetElementLocation(BarrelService* service, BarrelNode* node, uint index);
void barrel_TranscribeElement(BarrelService* service, BarrelNode* targetNode, void* target, unsigned int index, bool isRead);
void barrel_TranscribeSpan(CollectionRequest request);

size_t barrel_VectorRemainingSizeCap(BarrelNode* node);
uint barrel_VectorRemainingUnitCap(BarrelNode* node);

bool barrel_DeltaSizeRequest(BarrelService* service, BarrelNode* node, int delta);

int barrel_NextAvailableNode(BarrelService* service);
int barrel_maximumAvailableBarrels(BarrelService* barrelService, int requested);

bool barrel_ReadFromVector(CollectionRequest request);
bool barrel_WriteToVector(CollectionRequest request);
bool barrel_Insert(CollectionRequest request);
bool barrel_Remove(CollectionRequest request);
bool barrel_RemoveAt(CollectionRequest request);

bool barrel_Iterate(CollectionRequest* request);
bool barrel_Transcribe(CollectionRequest request);
bool barrel_ResizeRequest(CollectionRequest request);
bool barrel_Modify(CollectionRequest request);

void barrelRoll(BarrelService* service, int index, int delta, int dir);

DWORD WINAPI barrelRollingWork(void* target);
DWORD WINAPI barrelServiceWork(void* target);

void BarrelNode_ctor(BarrelService* service, BarrelNode* barrel, TypeID* type);
void BarrelServiceInit(BarrelService* barrelService, HeapService* heapService, ThreadHandle* threadBin, TypeID* barrelNodeTypeID);

bool barrel_RequestNode(BarrelService* barrelService, BarrelNode** nodeLoc, Vector vector);
bool barrel_RemoveNode(BarrelService* barrelService, BarrelNode* node);

CollectionExtensions barrel_TemplateExtension();