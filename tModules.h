#ifndef MODULES
#define MODULES

#include <tEnums.h>
#include <tDefines.h>
#include <Windows.h>

typedef struct {
	ullong _mem[512];
} Page;

typedef struct {
	ullong _mem[4];
} Barrel;

typedef struct {
	int _type;
	void* _ptr;
} Parameter;

typedef union {
	void* PTR;
	size_t VAL;
} PARAM;

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
	//const int _flags;
} Chunk;

typedef struct { // What kind of data is expected to be there
	const char* const _name;
	const size_t _size;
	const void* const _nullLoc;
	//const unsigned int _unitsPerBlock;
	//const unsigned int _barrelsPerBlock;
} TypeInfo;

typedef struct { // What global class of data is being represented
	const char* const _name;
	const TypeInfo* const _type;
} Property;

typedef Property* PROPERTY;

typedef struct { // How to manage the collection
	const TypeInfo* const _type;
	int (*_methods)(REQUEST request);
	int _memFlags;
} CollectionExtensions;

typedef struct { // Base class for collections
	const CollectionExtensions* const _extensions;
	uint _count;
	volatile LONG _spinLock;
} Collection;

typedef struct { // Fixed chunk of memory
	Collection _collection;
	Chunk _chunk;
} Bucket;

typedef struct {
	Link _link;
	char* _key;
	void* _occupant;
} hash_node;

typedef struct {
	Link _link;
	REQUEST _request;
} LinkedRequest;

typedef Collection* COLLECTION;

typedef struct {
	Bucket _bucket;
	volatile ULONG _start;
	volatile ULONG _next;
} RollingQue;

typedef struct {
	Collection _collection;

	uint _barrelCount;
	uint _barrelOffset;
	int _barrelStart;
	int _nextNode; // next garbage/physical (Whether is active or not)
} BarrelNode;

typedef struct {
	CollectionExtensions* _collection;
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
	//tService _barrelNodes;
	//Bucket _threadBin;
	//RollingQue _requests;

	HeapService* _heap;
	BarrelNode Omegus;

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

// Object Instance
typedef struct {
	uint _poolIndex;
	uint _mutationMask;
} Mutant;

// Class Mask
typedef struct {
	uint _count;
	uint _poolOffset;
	uint* _mutOffsets;
} Mutation;

// Backing Field Supplier
typedef struct {
	const char* const _name;
	uint _propCount;
	void* _propData;
	Property* _propDefs;
	//Bucket _propData, _propDefs;
} Mutagen;

typedef struct {
	uint _mutagenCount;
	uint _mutationCount;
	Mutagen* _mutagens;
	Mutation* _mutations;
	//Bucket _mutagens, _mutations;
	BarrelNode* _mutants, *_reverseLookup;
} Mutable;

/*

void foo(struct Mutagen, struct Mutation, int index)
int offSet = 0;
for (int m = Mutation.mask; m < totalMutations; m++)
	for (int i = 0; i < totalMutagens; i++)
		if ((1 << i) & m)
			offSet += Mutations;

void* attr = Mutagen[index - offset]

*/

#endif // !MODULES
