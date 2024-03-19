#ifndef tBarrel
#define tBarrel

#include <tHeap.h>
#include <tLink.h>
#include <tRequest.h>

BarrelService GlobalBarrelService;
TypeInfo GlobalBarrelNodeType;

ThreadHandle barrelService_ThreadBin[MaxThreadCount];
Request barrelService_QueBin[MaxQueCount];

BarrelNode* barrel_NodeLocation(int index);
BarrelNode* barrel_LastPhysicalNode();

void* barrel_GetElementLocation(BarrelNode* node, uint index);


size_t barrel_RemainingSizeCap(BarrelNode* node);
uint barrel_RemainingUnitCap(BarrelNode* node);

bool barrel_DeltaSize(Request request);
bool barrel_Resize(Request request);
void barrel_AppendRequest(LinkedRequest* linkedRequest);

int barrel_NextAvailableNode();
int barrel_maximumAvailableBarrels(int requested);

bool barrel_Head(Request request);
bool barrel_Iterate(Request* request);

bool barrel_Read(Request request);
bool barrel_Write(Request request);
bool barrel_Insert(Request request);
bool barrel_Remove(Request request);
bool barrel_RemoveAt(Request request);

bool barrel_Extensions(Request* request);

void barrelRoll(int index, int delta, int dir);

List<int>

DWORD WINAPI barrelRollingWork(void* target);
DWORD WINAPI barrelServiceWork(void* target);

BarrelNode BarrelNode_ctor(TypeInfo* type);
bool BarrelServiceInit(HeapService* heapService);

bool barrel_Construct(Request request);
bool barrel_RemoveNode(BarrelNode* node);

uint barrel_NodeCount();

#endif // !tBarrel