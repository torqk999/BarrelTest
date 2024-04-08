#ifndef MODULES
#define MODULES

#include <tEnums.h>
#include <tDefines.h>
#include <Windows.h>

typedef struct {
	char _type;
	void* _ptr;
} Preem;

typedef struct _Link {
	struct _Link* _next;
	struct _Link* _prev;
	volatile LONG _lock;
} Link;

typedef struct {
	Link _link;
	char* _key;
	void* _occupant;
} hash_node;

typedef struct {
	ullong _mem[512];
} Page;

typedef struct {
	ullong _mem[4];
} Barrel;

typedef struct {
	const size_t _size;
	const char* const _name;
} TypeRaw;

typedef struct {
	const TypeRaw _type;
	void* const _nullLoc;
	const unsigned int _flags;
	// Barrel Section
	const unsigned int _unitsPerBlock;
	const unsigned int _barrelsPerBlock;
} TypeInfo;

typedef struct {

	RequestType _type;

	void* _trg;
	void* _src;

	int _trgIx;
	int _srcIx;
	long long _size;

	//size_t _size;

} Request;

typedef struct {
	Link _link;
	Request _request;
} LinkedRequest;

typedef struct {
	TypeInfo* _info;
	bool(*_extensions)(Request* request);
	uint _count;
	uint _capacity;
} Collection;

typedef struct {
	Collection _collection;
	volatile LONG _userCount;
	volatile LONG _pointerCount;
} ManagedCollection;

typedef Collection* COLLECTION;

typedef struct {
	Collection* _src;
	void* _hLoc;
	void* _tLoc;
	size_t _hMem;
	size_t _tMem;
} Slice;

typedef struct {
	Collection _collection;
	void* _bucket;
} Bucket;

typedef struct {
	Bucket _bucket;
	volatile ULONG _start;
	volatile ULONG _next;
} RollingQue;

typedef struct {

	ManagedCollection _managed;
	LinkedRequest* _requests;

	BarrelFlags _flags;

	//uint _barrelCount = _collection._capacity
	uint _barrelOffset;
	int _barrelStart;
	int _nextNode; // next garbage/physical (Whether is active or not)
	
} BarrelNode;

typedef struct {
	RequestType _type;

	Bucket _init;
} BarrelRequest;

typedef struct {
	Collection* _collection;
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
} ThreadHandle;

typedef struct {
	int _localFlags;
	ThreadHandle _serviceThread;
} tService;

typedef struct {
	tService _pages;
	void* _heapStart;
	void* _heapEnd;
	unsigned int _pageCount;
} HeapService;

typedef struct {
	tService _barrelNodes;

	HeapService* _heap;

	BarrelNode Omegus;
	Bucket _threadBin;
	//RollingQue _requests;

	int _nextAvailable;
	int _lastPhysicalNode;
} BarrelService;

typedef struct {
	char* _key;
	void* _value;
} KeyPair;

typedef struct {
	TypeInfo _type;
	void* _target;
} Target;

typedef struct {
	TypeRaw _type;
	const char* _name;
} PropertyID;

typedef struct {
	PropertyID _property;
	Bucket _params;
} Parameter;

typedef struct {
	unsigned long long _firstBarCode;
	Bucket _subscriptions;
} Batch;

typedef struct {
	PropertyID _property;
	Bucket _batches;
} Allocator;

typedef struct {
	hash_map _map;
	Collection* _data;
} HashTable;

typedef struct {
	barrel_map _barrels;
	HashTable _types;
	HashTable _propAllocations;
} DataBase;

#endif // !MODULES
