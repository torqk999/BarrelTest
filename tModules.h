#pragma once
#include <tHelpers.h>

enum Constants {
	Vector_Default_Capacity = 16,
	Barrel_Max_Capacity = 1024,
	Hash_Min_Threshold = 30,
	Hash_Default_Threshold = 70,
	Hash_Max_Threshold = 80,
	Hash_Max_Conversion = 64,
	Heap_Size_Page = 4096,
	Heap_Page_Max = 128,
	Barrel_Size_Unit = 32,
	Barrel_Count_Node = 3,
	Heap_Threshold = 2048
};

//type and prop names
#define TERMINATE "nullTerm"
#define TARG_LINK "targLink"

const char* Reserved_Types[] = {
	TERMINATE,
	TARG_LINK
};

enum VectorType {
	NULL_VEC = 0,
	STACK_VEC = 1,
	CONST_VEC = 2,
	READ_ONLY = 4,
	BARREL = 8,
	LINKED = 16,
	REFERENCE = 32,
	ROOT = 64
};

typedef struct {
	size_t _size;
	const char* _name;
	unsigned int _flags;
} TypeID;

typedef struct _Link {
	struct _Link* _next;
	struct _Link* _prev;
	void* _self;
} Link;

typedef struct {
	Link _link;
	char* _key;
	void* _occupant;
} hash_node;

typedef struct {
	TypeID _type;
	void* _bucket;
	unsigned int _count;
} Vector;

typedef struct {
	Link _link;
	Vector _vector;
} LinkedVector;

typedef struct{
	ullong _mem[4];
} Barrel;

typedef struct barrelNode {
	Vector _vector;
	Link _bLink;

	int _locked;
	int _flags;
	int _barrelStart;
	int _barrelCount;

	int _blockOffset;
	int _requests;
	int _unitsPerBlock;
	int _barrelsPerBlock;
} BarrelNode;

static Barrel* _heapHead;
static BarrelNode* _nodeHead;
static unsigned int _nodeCount;

typedef struct {
	Vector _vector;
	float _threshold;
	unsigned int _first;
	unsigned int _last;
	unsigned int _capacity;
	int _lastLookupPerformance;
	unsigned int (*_hashFunc)(void *, unsigned int);
} hash_map;

typedef struct {
	hash_map _map;
	int _requests;
	unsigned long long _pageCount;
} barrel_map;

typedef struct {
	char* _key;
	void* _value;
} KeyPair;

typedef struct {
	TypeID _type;
	void* _target;
} Target;

typedef struct {
	TypeID _type;
	const char* _name;
} PropertyID;

typedef struct {
	PropertyID _property;
	Vector _params;
} Parameter;

typedef struct {
	unsigned long long _firstBarCode;
	Vector _subscriptions;
} Batch;

typedef struct {
	PropertyID _property;
	Vector _batches;
} Allocator;

typedef struct {
	hash_map _map;
	Vector _data;
} HashTable;

typedef struct {
	barrel_map _barrels;
	HashTable _types;
	HashTable _propAllocations;
} DataBase;

#define TYPE_ID(typeName) (TypeID) {sizeof(typeName),#typeName}