#pragma once
#include <tVector.h>
#include <tLink.h>

static TypeID* GlobalHashNodeType;

uint hashCapacity(uint request)
{
	uint output = 1;
	while (output < request)
		output *= 2;
	return output;
}

hash_node* hashMap_GetAtIndex(hash_map* map, uint index)
{
	if (index > map->_vector._capacity)
		return NULL;

	return &(((hash_node*)(map->_vector._bucket))[index]);
}

hash_node* hashMap_Start(hash_map* map)
{
	return hashMap_GetAtIndex(map, 0);
}

unsigned int GetCharHash(const char* key, unsigned int capacity)
{
	unsigned int buffer = 0;
	for (int i = 0; i < Hash_Max_Conversion; i++)
	{
		if (key[i] == '\0')
			break;
		buffer += key[i];
	}

	buffer = capacity == 0 ? buffer : buffer % capacity;

	return buffer;
}

unsigned int GetULLongHash(unsigned long long* key, unsigned int capacity)
{
	//unsigned int buffer = 0;
	//for (int i = 0; i < Hash_Max_Conversion; i++)
	//{
	//	buffer += key % 10;
	//}
	//
	//buffer = capacity == 0 ? buffer : buffer % capacity;

	// Soopah lazy ....

	return *key % capacity;
}

void hashNode_ctor(hash_node* node)
{
	node->_key = NULL;
	node->_link._prev = NULL;
	node->_link._next = NULL;
}

int hashMap_GetNode(hash_map* map, hash_node** nodePtr, uint* index, const char* key, bool assert)
{
	unsigned int startIndex = map->_hashFunc(key, map->_vector._capacity);//GetCharHash(key, map->_capacity);
	
	map->_lastLookupPerformance = -1; // Assume off map to start;

	for (int i = startIndex; i <= map->_vector._capacity; i++)
	{
		if (i == map->_vector._capacity)
		{
			// Flew off the end of map, maybe just wrap around?
			// Capacity should always be big enough for it to go somewhere.
			map->_lastLookupPerformance = -1;
			break;
		}
		
		// How bad is the map performing per instance basis, will need to average
		map->_lastLookupPerformance = i - startIndex; 

		hash_node* nextNode = hashMap_GetAtIndex(map, i);

		if (!(nextNode->_key))
		{
			if(assert)
			{
				nextNode->_key = (char*)key; // New key added to map
				*nodePtr = nextNode;
			}
			else
			{
				map->_lastLookupPerformance = -2; // Wrong key, assertion not made
			}
			break;
		}

		char* debugTest = nextNode->_key;

		if (strcmp((const char*)nextNode->_key, key) == 0)
		{
			*nodePtr = nextNode;
			break;
		}
	}
		
	return map->_lastLookupPerformance;
}

//bool hashMap_NodeOccupied(hash_map* map, const char* key, hash_node** nodePtr, bool assert)
//{
//	return hashMap_GetNode(map, nodePtr, key,, assert) > -1;
//}

bool hashMap_AssertionWillExceed(hash_map* map)
{
	return (float)(map->_vector._count + 1) / map->_vector._capacity > map->_threshold;
}

bool hashMap_GetPreviousNode(hash_map* map, unsigned int startIndex, hash_node** target)
{
	for (int i = startIndex; i > 0; i--)
	{
		hash_node* prevNode = hashMap_GetAtIndex(map, i);
		if (prevNode->_key)
		{
			*target = prevNode;
			return true;
		}
	}
		
	*target = NULL;
	return false;
}

bool hashMap_GetNextNode(hash_map* map, unsigned int startIndex, hash_node** target)
{
	for (int i = startIndex; i < map->_vector._capacity; i++)
	{
		hash_node* nextNode = hashMap_GetAtIndex(map, i);

		if (nextNode->_key)
		{
			*target = nextNode;
			return true;
		}
	}
		
	*target = NULL;
	return false;
}

void hashMap_EraseNode(hash_map* map, unsigned int index, void* target)
{
	hash_node* toBeErased = hashMap_GetAtIndex(map, index);

	if (toBeErased->_link._next)
	{
		hash_node* previous;
		hashMap_GetPreviousNode(map, index, &previous);

		if (previous)
			previous->_link._next = toBeErased->_link._next;
	}

	toBeErased->_link._next = NULL;
	toBeErased->_link._prev = NULL;
	toBeErased->_key = '\0';

	map->_vector._count--;
}

void hashMap_OccupyNode(hash_map* map, hash_node* occupation, uint index, void* occupant)
{
	hash_node *prev, *next;

	hashMap_GetPreviousNode(map, index, &prev);
	hashMap_GetNextNode(map, index, &next);

	if (occupant && !(occupation->_occupant))
		LinkElements(&(occupation->_link), &(prev->_link), &(next->_link));

	if (!occupant && occupation->_occupant)
		UnLink(&(occupation->_link));

	occupation->_occupant = occupant;

	map->_vector._count++;
	map->_first = map->_vector._count == 1 || map->_first > index ? index : map->_first;
}

int hashMap_Assert(hash_map* map, const char* key, void* occupant)
{
	//unsigned int trgNodeIndex;
	hash_node* targetNode;
	uint targetIndex;

	if (hashMap_GetNode(map, &targetNode, &targetIndex, key, true) < 0)
		return -1; // Lookup problem (ran off map, wrong key failed assertion)

	if (hashMap_AssertionWillExceed(map))
	{
		return -2;// Map max capacity

		//if (table->_capacity == 0)
		//	ResizeHash0(table, Vector_Default_Capacity);
		//else if (table->_count + 1 > Barrel_Max_Capacity)
		//	return -2; // Map max capacity
		//else
		//	DoubleHash(table);
	}

	//hashMap_GetAtIndex(map, trgNodeIndex)->_key = (char*)key;
	hashMap_OccupyNode(map, targetNode, targetIndex, occupant);
	return 1;
}

bool hashMap_GetOccupantLocation(hash_map* map, const char* key, void** occupantLocation)
{
	hash_node* node;
	uint index;
	if (hashMap_GetNode(map, &node, &index, key, false) < 0)
		return false;

	*occupantLocation = node->_occupant;
	return true;
}

void hashMap_Transcribe(hash_map* map, hash_node* oldNodes, unsigned int oldCapacity)
{
	//printf("[[[MAP TRAN-SCRIBE]]]\n");
	map->_vector._count = 0;
	for (int i = 0; i < oldCapacity; i++)
	{
		if (!oldNodes[i]._key)
		{
			//printf("oldNode %i not occupied\n", i);
			continue;
		}
			
		if (!oldNodes[i]._key)
		{
			//printf("oldNode %i missing key!\n", i);
			continue;
		}

		//printf("oldTableKey: %s\n", oldTable[i]._key);
		unsigned int index;
		hash_node* node;
		if (!hashMap_GetNode(map, &node, &index, oldNodes[i]._key, true))
			continue;

		if (map->_lastLookupPerformance < 0)
		{
			//printf("problem translating old member %s\n", oldTable[i]);
			continue;
		}

		hashMap_OccupyNode(map, node, index, oldNodes[i]._occupant);
	}
}

void hashMap_ctor1(hash_map* map, unsigned int flags, unsigned int capacity, float threshold, void* tableStart, unsigned int (*hashFunc)(void*, unsigned int))
{

	//capacity = Clamp(capacity, 0, Barrel_Max_Capacity);
	threshold = Clamp(threshold, (float)Hash_Min_Threshold / 100, (float)Hash_Max_Threshold / 100);
	Vector_ctor(&(map->_vector), GlobalHashNodeType, 0, capacity, tableStart);

	map->_hashFunc = hashFunc;

	for (uint i = 0; i < capacity; i++)
		hashNode_ctor(hashMap_GetAtIndex(map, i));

	map->_threshold = threshold;

	map->_first = 0;
	map->_last = 0;
	map->_lastLookupPerformance = 0;
}

void hashMap_ctor0(hash_map* map, TypeFlags flags, void* mapStart, unsigned int (*hashFunc)(void*, unsigned int))
{
	hashMap_ctor1(map, flags, Vector_Default_Capacity, Hash_Default_Threshold, mapStart, hashFunc);
}

hash_map hashMap_Create(unsigned int capacity, TypeFlags flags, float threshold, void* mapStart, unsigned int (*hashFunc)(void*, unsigned int))
{
	hash_map output;
	hashMap_ctor1(&output, flags, capacity, threshold, mapStart, hashFunc);
	return output;
}

#define HASH_MAP(capacity, threshold) hashMap_Create(capacity, BUCKET, threshold, (hash_node[capacity]){0}, GetCharHash)

//#define HASH_ASSERT(tablePtr, key, value) hashMap_Assert(tablePtr, key, sizeof(value), &value) 

//void hash_dtor(hash_map* table)
//{
//	hashFreeMap(table);
//}

