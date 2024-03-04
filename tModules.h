#ifndef MODULES
#include <tDefines.h>
#include <tEnums.h>
#include <Windows.h>

typedef struct {
	const char* name;
	int age;
	float height;
	unsigned long long _SIN;
} CharacterSheet;

typedef struct node {
	CharacterSheet* _sheet;
	struct node* _parent;
	struct node* _children;
	struct node* _sibling;
	int _childCount;

} Node;

/*const char* Reserved_Types[] = {
	TERMINATE,
	TARG_LINK
};*/


typedef struct {
	void* _trg;
	void* _src;
	uint _trgIx;
	uint _srcIx;
	int _count;
	CheckType _type;
	size_t _size;
	void* _service;
} CollectionRequest;

typedef struct {
	bool (*Iterate)(CollectionRequest* request);
	bool (*Compare)(CollectionRequest request);
	bool (*Transcribe)(CollectionRequest request);
	bool (*Modify)(CollectionRequest request);
	void* _singleBuffer;
} CollectionExtensions;

typedef struct {
	const size_t _size;
	const char* _name;
	const unsigned int _flags;

	// Tranformation Section
	const CollectionExtensions _extensions;

	// Barrel Section
	const unsigned int _unitsPerBlock;
	const unsigned int _barrelsPerBlock;
} TypeID;

typedef struct {
	const TypeID* _type;
	uint _count;
	uint _capacity;
} Collection;

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
	unsigned int _capacity;
	void* _bucket;
} Vector;

typedef struct {
	ullong _mem[512];
} Page;

typedef struct {
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
	uint _start;
	uint _end;
} RollingQue;

typedef struct {

	Collection _vector;

	BarrelFlags _flags;
	volatile LONG _userCount;
	volatile LONG _pointerCount;

	uint _barrelOffset;
	int _barrelStart;
	int _nextNode;
	int _requests;

} BarrelNode;

typedef struct {
	Vector _vector;
	float _threshold;
	unsigned int _first;
	unsigned int _last;
	int _lastLookupPerformance;
	unsigned int (*_hashFunc)(void*, unsigned int);
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

#endif // !MODULES
#define MODULES