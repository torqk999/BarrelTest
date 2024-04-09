#ifndef MODULES
#define MODULES

#include <tEnums.h>
#include <tDefines.h>
#include <Windows.h>

typedef struct {
	ParamType _type;
	void* _ptr;
} Parameter;

typedef struct {
	const RequestType _type;
	void** _params;
} REQUEST;

typedef struct _Link {
	struct _Link* _next;
	struct _Link* _prev;
	volatile LONG _lock;
} Link;

typedef struct { // Contextualized chunk of memory
	const size_t _head;
	const size_t _size;
	const int _flags;
} Chunk;

typedef struct { // What kind of data is expected to be there
	const char* const _name;
	const size_t _size;
	const void* const _nullLoc;
	const unsigned int _unitsPerBlock;
	const unsigned int _barrelsPerBlock;
} TypeInfo;

typedef struct { // What global class of data is being represented
	const char* const _name;
	const TypeInfo* const _type;
} Property;

typedef struct { // How to manage the data
	const TypeInfo* const _type;
	int (*_extensions)(REQUEST request);
} Collection;

typedef struct { // Fixed chunk of memory
	Collection _collection;
	Chunk _chunk;
	uint _count;
} Bucket;

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
	Link _link;
	REQUEST _request;
} LinkedRequest;

typedef struct {
	Collection _collection;
	volatile LONG _userCount;
	volatile LONG _pointerCount;
} ManagedCollection;

typedef Collection* COLLECTION;

typedef struct {
	Bucket _bucket;
	volatile ULONG _start;
	volatile ULONG _next;
} RollingQue;

typedef struct {
	ManagedCollection _managed;
	LinkedRequest* _requests;

	BarrelFlags _flags;

	uint _barrelCount;
	uint _barrelOffset;
	int _barrelStart;
	int _nextNode; // next garbage/physical (Whether is active or not)
} BarrelNode;

typedef struct {
	Collection* _collection;
	float _threshold;
	unsigned int _first;
	unsigned int _last;
	int _lastLookupPerformance;
} HashMap;

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

//typedef struct {
//	Property _property;
//	Bucket _params;
//} ParamType;

typedef struct {
	unsigned long long _firstBarCode;
	Bucket _subscriptions;
} Batch;

typedef struct {
	Property _property;
	Bucket _batches;
} Allocator;


#endif // !MODULES
