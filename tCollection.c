#include <tCollection.h>
//
////LinkedVector* LinkedVector_GetTermNode(LinkedVector* vector)
////{
////	LinkedVector* linked = (LinkedVector*)(vector->_link._next);
////
////	while (linked)
////		linked = (LinkedVector*)(linked->_link._next);
////
////	return vector;
////}
////unsigned int LinkedVector_GetCount(LinkedVector* vector)
////{
////	unsigned int _output = 0;
////
////	_output += vector->_vector._count;
////
////	if (vector->_link._next)
////		_output += LinkedVector_GetCount((LinkedVector*)(vector->_link._next));
////
////	return _output;
////}
////void* LinkedVector_Iterate(LinkedVector** vector, void* current, int delta)
////{
////	size_t trg = (size_t)current + (delta * (*vector)->_vector._type->_size);
////	size_t start = (size_t)((*vector)->_vector._bucket);
////	long long index = Vector_GetIndex(&((*vector)->_vector), (void*)trg);
////
////
////	if (index < 0)
////	{
////		if (!(*vector)->_link._prev)
////			return NULL;
////		*vector = (LinkedVector*)((*vector)->_link._prev);
////		index += (*vector)->_vector._count;
////		trg = (size_t)((*vector)->_vector._bucket) + (index * (*vector)->_vector._type->_size);
////		return LinkedVector_Iterate(vector, (void*)trg, 0);
////	}
////
////	if (trg >= start + (((*vector)->_vector._count) * (*vector)->_vector._type->_size))
////	{
////		if (!(*vector)->_link._next)
////			return NULL;
////		*vector = (LinkedVector*)((*vector)->_link._next);
////		index -= (*vector)->_vector._count;
////		trg = (size_t)((*vector)->_vector._bucket) + (index * (*vector)->_vector._type->_size);
////		return LinkedVector_Iterate(vector, (void*)trg, 0);
////	}
////
////	return (void*)trg;
////}
////void LinkedVector_ctor(
////	LinkedVector* linked,
////	Link* next,
////	Link* prev,
////	TypeID type,
////	unsigned int existingCount,
////	void* existingBucket
////)
////{
////	Vector_ctor(&(linked->_vector), type, existingCount, existingBucket);
////
////	linked->_link._next = next;
////	linked->_link._prev = prev;
////}
////LinkedVector Vector_Vectorize(TypeID type, unsigned int existingCount, void* existingBucket)
////{
////	LinkedVector vector;
////
////	LinkedVector_ctor(&vector, NULL, NULL, type, existingCount, existingBucket);
////
////	return vector;
////}
////void Vector_Link(LinkedVector* trg, LinkedVector* link)
////{
////	if (trg)
////		trg->_link._next = link ? trg->_vector._type->_size == link->_vector._type->_size ? &(link->_link) : NULL : NULL;
////
////	if (link)
////		link->_link._prev = trg ? trg->_vector._type->_size == link->_vector._type->_size ? &(trg->_link) : NULL : NULL;
////}
//
//const TypeInfo NULL_TYPE = { 0, "NULL" };
//
//void* Collection_Iterate(Collection* trg, Request* iter)
//{
//	return trg->_extensions(iter);
//}
//void Managed_Use(		ManagedCollection* trg)
//{
//	InterlockedIncrement(&(trg->_userCount));
//}
//void Managed_Free(		ManagedCollection* trg)
//{
//	InterlockedDecrement(&(trg->_userCount));
//}
//void Managed_Point(		ManagedCollection* trg)
//{
//	InterlockedIncrement(&(trg->_pointerCount));
//}
//void Managed_Release(	ManagedCollection* trg)
//{
//	InterlockedDecrement(&(trg->_pointerCount));
//}
//
//bool Collection_GetSlice(Collection* trg, Slice* slice, uint start, uint count)
//{
//	return trg->_extensions(&(Request) { SLICE_CREATE, trg, slice, start, 0, count });
//}
//
//bool Collection_GetSlice(Collection* trg, Slice* slice)
//{
//	return false;
//}
//
//bool Collection_Transcribe(Collection* trg, Collection* src, unsigned int tIx, unsigned int sIx, unsigned int count) {
//	return trg->_extensions(&(Request) { TRANSCRIBE_COLLECTIONS, trg, src, tIx, sIx, count });
//}
//bool Collection_Resize(Collection* trg, unsigned int count) {
//	return trg->_extensions(&(Request) { MODIFY_DELTA_CAPACITY, trg, NULL, 0, 0, count });
//}

CollectionExtensions GlobalCollectionInfo[testTypeBinCount];
uint GlobalCollectionInfoCurrentCount = 0;

Bucket Collection_RawBucket(COLLECTION col, void* head, unsigned int count) {
	return (Bucket) {
		Collection_Create(Collection_ReadOnlyExtensions(col->_extensions), count),
		Chunk_Create(head, col->_extensions->_type->_size * count)
	};
}

bool Collection_Transcribe(COLLECTION trg, COLLECTION src, unsigned int trgStart, unsigned int srcStart, unsigned int count) {

	if (!Collection_InfoCompare(trg->_extensions, src->_extensions))
		return false;

	if (!Collection_Request(trg) || !Collection_Request(src))
		return false;

	void* trgPtr;
	REQUEST trgIter = Request(ITERATE, P_(tSRC, trg), P_(tTRG, &trgPtr), P_(Ix_TRG, &trgStart));

	void* srcPtr;
	REQUEST srcIter = Request(ITERATE, P_(tSRC, src), P_(tTRG, &srcPtr), P_(Ix_TRG, &srcStart));

	size_t size = trg->_extensions->_type->_size;

	for (int i = 0; i < count; i++) {
		trg->_extensions->_methods(trgIter);
		src->_extensions->_methods(srcIter);
		rawTranscribe(trgPtr, srcPtr, size);
	}

	Collection_Release(trg);
	Collection_Release(src);

	return true;
}

bool Collection_ReadSpan(COLLECTION src, void* trg, unsigned int start, unsigned int count) {
	Bucket trgBucket = Collection_RawBucket(src, trg, count);
	return Collection_Transcribe(src, &trgBucket, 0, start, count);
}
bool Collection_WriteSpan(COLLECTION trg, void* src, unsigned int start, unsigned int count) {
	Bucket srcBucket = Collection_RawBucket(trg, src, count);
	return Collection_Transcribe(trg, &srcBucket, start, 0, count);;
}

//bool Collection_InsertSpan(Collection* trg, void* src, unsigned int index, unsigned int count) {
//	return trg->_extensions(&(Request) { MODIFY_INSERT, trg, src, index, 0, count });
//}
//bool Collection_RemoveSpanAt(Collection* trg, unsigned int index, unsigned int count) {
//	return trg->_extensions(&(Request) { MODIFY_REMOVE_AT, trg, NULL, index, 0, count });
//}
//
bool Collection_Read(COLLECTION src, void* trg, unsigned int index) {
	return Collection_ReadSpan(src, trg, index, 1);
}
bool Collection_Write(COLLECTION trg, void* src, unsigned int index) {
	return Collection_WriteSpan(trg, src, index, 1);
}
uint Collection_Capacity(COLLECTION collection) {
	uint output;
	if (!collection->_extensions->_methods(Request(INFO, P_(tTRG, collection) , P_(tCAPACITY, &output))))
		return 0;
	return output;
}
uint Collection_Count(COLLECTION collection) {
	uint output;
	if (!collection->_extensions->_methods(Request(INFO, P_(tVARIANT, tCOUNT), P_(tSRC, collection), P_(tTRG, &output))))
		return 0;
	return output;
}
bool Collection_ReadOnly(COLLECTION collection) {
	return collection->_extensions->_memFlags & READ_ONLY;
}
inline bool Collection_Compare(COLLECTION a, COLLECTION b)
{
	return a->_extensions == b->_extensions;
}
inline bool Collection_InfoCompare(CollectionExtensions* trg, CollectionExtensions* src) {
	if (trg->_memFlags & READ_ONLY)
		return false;
	
	if (trg->_methods != src->_methods ||
		//A._memFlags != B._memFlags ||
		!TypeInfo_Compare(trg->_type, src->_type))
		return false;
	return true;
}
inline bool Collection_Request(COLLECTION collection)
{
	while (InterlockedCompareExchange(&(collection->_spinLock), 1, 0)) {
		// time sensitive logic...
		// eg:
		// int counter = 0;
		// if (counter > 100000)
		//	return false;
		// counter++;
	}
	return true;
}
inline bool Collection_Release(COLLECTION collection)
{
	InterlockedExchange(&(collection->_spinLock), 0);
	return true;
}
//bool Collection_Insert(Collection* trg, void* src, unsigned int index)
//{
//	return Collection_InsertSpan(trg, src, index, 1);
//}
//bool Collection_RemoveAt(Collection* trg, unsigned int index)
//{
//	return Collection_RemoveSpanAt(trg, index, 1);
//}
//
//bool Collection_RemoveSpan(Collection* trg, void* search, unsigned int count)
//{
//	return trg->_extensions(&(Request) { MODIFY_REMOVE_FIRST_FOUND, trg, search, NULL, 0, count });
//}
//
//bool Collection_Remove(Collection* trg, void* search)
//{
//	return Collection_RemoveSpan(trg, search, 1);
//}
//
//void* Collection_Location(Collection* src, uint index){
//	Request req = (Request){ LOCATION, NULL, src, 0, index };
//	bool result = src->_extensions(&req);
//	return result ? req._trg : NULL;
//}
//



CollectionExtensions CollectionExtensions_Create(TypeInfo* type, bool(*extensions)(RequestType* request), int memFlags) {
	return (CollectionExtensions) {
		type,
		extensions,
		memFlags
	};
}

Collection Collection_Create(CollectionExtensions* extensions, uint count)
{

	return (Collection) {
		extensions,
		count,
		0
	};
}

bool Collection_RawMethods(REQUEST request) {
	Bucket* rawBucket = request._params[tSRC];
	switch (request._type) {
	case ITERATE:;
		uint index = request._params[Ix_TRG];
		request._params[tTRG] = index >= rawBucket->_collection._count ? NULL : rawBucket->_chunk._head + (index * rawBucket->_collection._extensions->_type->_size);
		((size_t)request._params[Ix_TRG])++;
		return true;
	default:
		return false;
	}
}

CollectionExtensions* Collection_GetRawExtensions(CollectionExtensions* src) {
	return Collection_GetExtensions(src->_type, Collection_RawMethods, src->_memFlags | READ_ONLY);
}

CollectionExtensions* Collection_GetExtensions(TypeInfo* typeInfo, bool(*methods)(RequestType* request), int memFlags)  {

	CollectionExtensions newInfo = {
		typeInfo,
		methods,
		memFlags
	};

	for (int i = 0; i < GlobalCollectionInfoCurrentCount; i++) {
		if (Collection_InfoCompare(&GlobalCollectionInfo[i], &newInfo)) {
			PREENT("CollectionInfo found!\n");
			return &GlobalCollectionInfo[i];
		}
			
	}

	if (GlobalCollectionInfoCurrentCount >= testTypeBinCount)
	{
		PREENT("No type slots available...\n");
		return NULL;
	}

	CollectionExtensions* ptr = &GlobalCollectionInfo[GlobalCollectionInfoCurrentCount];
	rawTranscribe(ptr, &newInfo, sizeof(TypeInfo));

	PREENT("Created new CollectionInfo!\n");

	GlobalCollectionInfoCurrentCount++;
	return ptr;
}

//ManagedCollection Managed_ctor(Collection collection)
//{
//	ManagedCollection newManaged;
//
//	newManaged._collection = collection;
//	newManaged._userCount = 0;
//	newManaged._pointerCount = 0;
//
//	return newManaged;
//}
//
//bool Collection_dtor(Collection* collection){
//
//	return collection->_extensions(&(Request) { DECON, collection});
//}
