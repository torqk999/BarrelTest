#pragma once
#include <tVector.h>
#include <tLink.h>

static TypeID* GlobalHashNodeType;

#define HASH_MAP(capacity, threshold) hashMap_Create(capacity, BUCKET, threshold, (hash_node[capacity]){0}, GetCharHash)


uint hashCapacity(uint request);

hash_node* hashMap_GetAtIndex(hash_map* map, uint index);

hash_node* hashMap_Start(hash_map* map);

unsigned int GetCharHash(const char* key, unsigned int capacity);

unsigned int GetULLongHash(unsigned long long* key, unsigned int capacity);

void hashNode_ctor(hash_node* node);

int hashMap_GetNode(hash_map* map, hash_node** nodePtr, uint* index, const char* key, bool assert);

//bool hashMap_NodeOccupied(hash_map* map, const char* key, hash_node** nodePtr, bool assert)
//{
//	return hashMap_GetNode(map, nodePtr, key,, assert) > -1;
//}

bool hashMap_AssertionWillExceed(hash_map* map);

bool hashMap_GetPreviousNode(hash_map* map, unsigned int startIndex, hash_node** target);

bool hashMap_GetNextNode(hash_map* map, unsigned int startIndex, hash_node** target);

void hashMap_EraseNode(hash_map* map, unsigned int index, void* target);

void hashMap_OccupyNode(hash_map* map, hash_node* occupation, uint index, void* occupant);

int hashMap_Assert(hash_map* map, const char* key, void* occupant);

bool hashMap_GetOccupantLocation(hash_map* map, const char* key, void** occupantLocation);

void hashMap_Transcribe(hash_map* map, hash_node* oldNodes, unsigned int oldCapacity);

void hashMap_ctor1(hash_map* map, unsigned int flags, unsigned int capacity, float threshold, void* tableStart, unsigned int (*hashFunc)(void*, unsigned int));

void hashMap_ctor0(hash_map* map, TypeFlags flags, void* mapStart, unsigned int (*hashFunc)(void*, unsigned int));

hash_map hashMap_Create(unsigned int capacity, TypeFlags flags, float threshold, void* mapStart, unsigned int (*hashFunc)(void*, unsigned int));


//#define HASH_ASSERT(tablePtr, key, value) hashMap_Assert(tablePtr, key, sizeof(value), &value) 

//void hash_dtor(hash_map* table)
//{
//	hashFreeMap(table);
//}

