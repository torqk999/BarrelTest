#pragma once
#include <tHelpers.h>
#include <tPreent.h>
#include <Windows.h>

enum Constants {
	NONE = -2,
	OMEGA = -1,

	Vector_Default_Capacity = 16,
	Barrel_Max_Capacity = 1024,
	Hash_Min_Threshold = 30,
	Hash_Default_Threshold = 70,
	Hash_Max_Threshold = 80,
	Hash_Max_Conversion = 64,
	//Heap_Size_Page = 4096,
	//Heap_Page_Max = 128,
	//Barrel_Size_Unit = 32,
	//Barrel_Count_Node = 3,
	//Heap_Threshold = 2048
};

enum ThreadType
{
	THREAD,
	HEAP,
	BARREL
};

//type and prop names
#define TERMINATE "nullTerm"
#define TARG_LINK "targLink"
#define QUE_SIZE 16

#define MaxThreadCount 8
#define MaxPageCount 64
#define PageSize 4096
#define PageThreshold 2048

const char* Reserved_Types[] = {
	TERMINATE,
	TARG_LINK
};

typedef enum {
	DONE = 0,
	RUN = 1,
	req_INIT = 2,
	req_ROLL = 4,
	req_FREE = 8,
	//req_RESIZE = 16,
	wait_ROLL = 32,
	wait_FREE = 64
} BarrelFlags;

typedef enum {
	NULL_VEC = 0,
	STACK_VEC = 1,
	CONST_VEC = 2,
	READ_ONLY = 4,
	ALLOCED = 8,
	LINKED = 16,
	REFERENCE = 32,
	ROOT = 64
} VectorType;

typedef struct {
	const size_t _size;
	const char* _name;
	const unsigned int _flags;

	// Barrel Section
	const unsigned int _unitsPerBlock;
	const unsigned int _barrelsPerBlock;
} TypeID;

typedef struct _Link {
	struct _Link* _next;
	struct _Link* _prev;
} Link;

typedef struct {
	Link _link;
	char* _key;
	void* _occupant;
} hash_node;

typedef struct {
	const TypeID* _type;
	unsigned int _count;
	const unsigned int _locality;
	void* _bucket;
} Vector;

typedef struct {
	const TypeID* _type;
	int _count;
	int UNUSED; // Padding for _locality
} BarrelVector;

typedef struct {
	Link _link;
	Vector _vector;
} LinkedVector;

typedef struct {
	ullong _mem[512];
} Page;

typedef struct{
	ullong _mem[4];
} Barrel;

typedef struct {
	//unsigned int _flags;
	//void* _target;
	int _delta;
	void* _target;
} QueRequest;

typedef struct {
	QueRequest _requests[QUE_SIZE];
	unsigned int _start;
	unsigned int _end;
} RollingQue;

typedef struct {
	BarrelVector _vector;

	int _nextNode;
	int _flags;
	int _barrelStart;
	int _barrelCount;
	int _barrelOffset;
	int _requests;

	volatile LONG _userCount;
} BarrelNode;

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
	HANDLE _handle;
	DWORD _ID;
	void* _service;
	int _offset;
	//int _flags;
} ThreadHandle;

typedef struct {
	int _localFlags;
	ThreadHandle _serviceThread;
	RollingQue _que;
} tService;

typedef struct {
	tService _pages;
	void* _heapStart;
	void* _heapEnd;
	unsigned int _pageCount;
} HeapService;

typedef struct {
	tService _barrelNodes;
	BarrelNode Omegus;
	HeapService* _heap;
	ThreadHandle* _threadBin;
	unsigned int _threadCount;
	int _nextAvailable;
	int _lastPhysicalNode;
} BarrelService;

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

