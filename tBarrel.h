#ifndef tBarrel
#define tBarrel

#include <tHeap.h>
#include <tCollection.h>

static bool GlobalBarrelServiceStarted = false;
static BarrelService GlobalBarrelService;
//TypeInfo GlobalBarrelNodeType;

ThreadHandle barrelService_ThreadBin[MaxThreadCount];
REQUEST barrelService_QueBin[MaxQueCount];

BarrelNode* Barrel_GetNode(int index);
BarrelNode* Barrel_GetLastPhysicalNode();

void* Barrel_GetBarrelPtr(uint index);
void* Barrel_GetUnitPtr(BarrelNode* node, uint index);

uint Barrel_NodeCount();

size_t Barrel_RemainingSizeCap(BarrelNode* node);
uint Barrel_RemainingUnitCap(BarrelNode* node);

//bool barrel_Use(REQUEST* request);

uint Barrel_maximumAvailableBarrels(uint requested);

bool Barrel_Roll(BarrelNode* node, int request);
bool Barrel_DeltaSize(REQUEST request);
bool Barrel_Resize(REQUEST request);
//void barrel_AppendRequest(LinkedRequest* linkedRequest);

int Barrel_NextAvailableNodeIx();


//bool barrel_Head(REQUEST request);
//bool barrel_Iterate(REQUEST* request);
//bool barrel_Read(REQUEST request);
//bool barrel_Write(REQUEST request);
//bool barrel_Insert(REQUEST request);
//bool barrel_Remove(REQUEST request);
//bool barrel_RemoveAt(REQUEST request);
//bool barrel_Location(REQUEST request);



inline bool Barrel_Manage(REQUEST request);
inline bool Barrel_Transcribe(REQUEST request);
inline bool Barrel_Info(REQUEST request);

bool Barrel_Extensions(REQUEST request);


void Barrel_NodeCtor(TypeInfo* info, BarrelNode* loc, void* head, int memFlags, uint initCapacity, uint startBarrel);
BarrelNode* Barrel_Sourced(TypeInfo* info, void* srcHead, int memFlags, uint initCapacity);


////////////////////
// Public Section //
////////////////////

bool Barrel_ServiceInit();

COLLECTION Barrel_ctor(const char* typeName, size_t unitSize, void* head, int memFlags, uint initCapacity);


//COLLECTION barrel_Construct(size_t size, const char* typeName, void* src);
//bool barrel_RemoveNode(BarrelNode* node);

//uint barrel_NodeCount();

#endif // !tBarrel