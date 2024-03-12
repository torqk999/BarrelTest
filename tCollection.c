#include <tCollection.h>

//LinkedVector* LinkedVector_GetTermNode(LinkedVector* vector)
//{
//	LinkedVector* linked = (LinkedVector*)(vector->_link._next);
//
//	while (linked)
//		linked = (LinkedVector*)(linked->_link._next);
//
//	return vector;
//}
//unsigned int LinkedVector_GetCount(LinkedVector* vector)
//{
//	unsigned int _output = 0;
//
//	_output += vector->_vector._count;
//
//	if (vector->_link._next)
//		_output += LinkedVector_GetCount((LinkedVector*)(vector->_link._next));
//
//	return _output;
//}
//void* LinkedVector_Iterate(LinkedVector** vector, void* current, int delta)
//{
//	size_t trg = (size_t)current + (delta * (*vector)->_vector._type->_size);
//	size_t start = (size_t)((*vector)->_vector._bucket);
//	long long index = Vector_GetIndex(&((*vector)->_vector), (void*)trg);
//
//
//	if (index < 0)
//	{
//		if (!(*vector)->_link._prev)
//			return NULL;
//		*vector = (LinkedVector*)((*vector)->_link._prev);
//		index += (*vector)->_vector._count;
//		trg = (size_t)((*vector)->_vector._bucket) + (index * (*vector)->_vector._type->_size);
//		return LinkedVector_Iterate(vector, (void*)trg, 0);
//	}
//
//	if (trg >= start + (((*vector)->_vector._count) * (*vector)->_vector._type->_size))
//	{
//		if (!(*vector)->_link._next)
//			return NULL;
//		*vector = (LinkedVector*)((*vector)->_link._next);
//		index -= (*vector)->_vector._count;
//		trg = (size_t)((*vector)->_vector._bucket) + (index * (*vector)->_vector._type->_size);
//		return LinkedVector_Iterate(vector, (void*)trg, 0);
//	}
//
//	return (void*)trg;
//}
//void LinkedVector_ctor(
//	LinkedVector* linked,
//	Link* next,
//	Link* prev,
//	TypeID type,
//	unsigned int existingCount,
//	void* existingBucket
//)
//{
//	Vector_ctor(&(linked->_vector), type, existingCount, existingBucket);
//
//	linked->_link._next = next;
//	linked->_link._prev = prev;
//}
//LinkedVector Vector_Vectorize(TypeID type, unsigned int existingCount, void* existingBucket)
//{
//	LinkedVector vector;
//
//	LinkedVector_ctor(&vector, NULL, NULL, type, existingCount, existingBucket);
//
//	return vector;
//}
//void Vector_Link(LinkedVector* trg, LinkedVector* link)
//{
//	if (trg)
//		trg->_link._next = link ? trg->_vector._type->_size == link->_vector._type->_size ? &(link->_link) : NULL : NULL;
//
//	if (link)
//		link->_link._prev = trg ? trg->_vector._type->_size == link->_vector._type->_size ? &(trg->_link) : NULL : NULL;
//}

const TypeID NULL_TYPE = { 0, "NULL" };

bool Collection_Iterate(Request* request)
{
	Bucket* vector = (Bucket*)request->_trg;
	return vector->_collection._extensions->Iterate(request);
}
bool Collection_Transcribe(Request request) {

	Collection* src = request._src;
	Collection* trg = request._trg;

	if (!TypeID_Compare((Request) { COMPARE_COMPATIBILITY_FULL, trg->_type, src->_type, NULL, 0, 0, 1 }))
		return false;

	if (request._trgIx + request._count >= trg->_count ||
		request._srcIx + request._count >= src->_count)
		return false;

	Request readRequest = { TRANSCRIBE_COLLECTION_TO_RAW, request._buffer, src, NULL, 0, request._srcIx, 1 };
	Request writeRequest = { TRANSCRIBE_RAW_TO_COLLECTION, trg, request._buffer, NULL, request._trgIx, 0, 1 };

	for (int i = 0; i < request._count; i++)
	{
		if (!src->_extensions->Transcribe(readRequest) ||
			!trg->_extensions->Transcribe(writeRequest))
			return false;

		readRequest._srcIx++;
		readRequest._trgIx++;

		writeRequest._srcIx++;
		writeRequest._trgIx++;
	}

	return true;
}
bool Collection_Resize(Collection* trg, unsigned int count)
{
	return trg->_extensions->Modify((Request) { MODIFY_RESIZE, trg, NULL, NULL, 0, 0, count });
}
bool Collection_ReadSpan(Collection* src, void* trg, unsigned int start, unsigned int count)
{
	return src->_extensions->Transcribe((Request) { TRANSCRIBE_COLLECTION_TO_RAW, trg, src, NULL, start, 0, count });
}
bool Collection_Read(Collection* src, void* trg, unsigned int index)
{
	return Collection_ReadSpan(trg, src, index, 1);
}
bool Collection_WriteSpan(Collection* trg, void* src, unsigned int start, int count) {

	return trg->_extensions->Transcribe((Request) { TRANSCRIBE_RAW_TO_COLLECTION, trg, src, NULL, start, 0, count });
}
bool Collection_Write(Collection* trg, void* src, unsigned int index)
{
	return Collection_WriteSpan(trg, src, index, 1);
}
bool Collection_InsertSpan(Collection* trg, void* src, unsigned int index, unsigned int count)
{
	return trg->_extensions->Modify((Request) { MODIFY_INSERT, trg, src, NULL, index, 0, count });
}
bool Collection_Insert(Collection* trg, void* src, unsigned int index)
{
	return Collection_InsertSpan(trg, src, index, 1);
}
bool Collection_RemoveSpan(Collection* trg, void* search, unsigned int count)
{
	return trg->_extensions->Modify((Request) { MODIFY_REMOVE_FIRST_FOUND, trg, search, NULL, 0, 0, count });
}
bool Collection_Remove(Collection* trg, void* search)
{
	return Collection_RemoveSpan(trg, search, 1);
}
bool Collection_RemoveSpanAt(Collection* trg, unsigned int index, unsigned int count)
{
	return trg->_extensions->Modify((Request) { MODIFY_REMOVE_AT, trg, NULL, NULL, index, 0, count });
}
bool Collection_RemoveAt(Collection* trg, unsigned int index)
{
	return Collection_RemoveSpanAt(trg, index, 1);
}


//CollectionExtensions CreateGenericExtensions(TypeFlags initFlags, CollectionExtensions* templateExtensions)
//{
//	CollectionExtensions newExtensions = *templateExtensions;
//
//	if (initFlags & ALPHA)
//		newExtensions.Compare = initFlags & POINTER ? defaultStringCompare : defaultCharCompare;
//
//	if (initFlags & INTEGRAL)
//		newExtensions.Compare = initFlags & UNSIGNED ? UnsignedIntegralCompare : SignedIntegralCompare;
//
//	if (initFlags & FLOATING)
//		newExtensions.Compare = FloatingCompare;
//
//	return newExtensions;
//}

Collection Collection_ctor(
	TypeID* type,
	CollectionExtensions* extensions,
	//bool(*compare)(Request),
	uint initCount,
	uint initCapacity
)
{
	Collection newCollection;

	newCollection._count = initCount;
	newCollection._capacity = initCapacity;
	newCollection._extensions = extensions;
	newCollection._type = type;
	//newCollection.Compare = compare;

	//CloneTypeID(&(newCollection._type), type);

	return newCollection;
}

bool Collection_dtor(Collection* collection){

	return collection->_extensions->Modify((Request) { MODIFY_DECON, collection});
}