#include <tVector.h>

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

bool Vector_Iterate(CollectionRequest* request)
{
	Vector* vector = (Vector*)request->_trg;
	return vector->_type->_extensions.Iterate(request);
}
bool Vector_Transcribe(CollectionRequest request) {

	Vector* src = request._src;
	Vector* trg = request._trg;

	if (!CompareTypes((CollectionRequest) { trg->_type, src->_type, 0, 0, 1, COMPARE_COMPATIBILITY_FULL }))
		return false;

	if (request._trgIx + request._count >= trg->_count ||
		request._srcIx + request._count >= src->_count)
		return false;

	CollectionRequest readRequest = { src->_type->_extensions._singleBuffer, src, 0, request._srcIx, 1, TRANSCRIBE_COLLECTION_TO_RAW };
	CollectionRequest writeRequest = { trg, src->_type->_extensions._singleBuffer, request._trgIx, 0, 1, TRANSCRIBE_RAW_TO_COLLECTION };

	for (int i = 0; i < request._count; i++)
	{
		if (!src->_type->_extensions.Transcribe(readRequest) ||
			!trg->_type->_extensions.Transcribe(writeRequest))
			return false;

		readRequest._srcIx++;
		readRequest._trgIx++;

		writeRequest._srcIx++;
		writeRequest._trgIx++;
	}

	return true;
}
bool Vector_Resize(Vector* trg, unsigned int count)
{
	return trg->_type->_extensions.Modify((CollectionRequest) { trg, NULL, 0, 0, count, MODIFY_RESIZE });
}

bool Vector_ReadSpan(Vector* src, void* trg, unsigned int start, unsigned int count)
{
	return src->_type->_extensions.Transcribe((CollectionRequest) { trg, src, start, 0, count, TRANSCRIBE_COLLECTION_TO_RAW });
}
bool Vector_Read(Vector* src, void* trg, unsigned int index)
{
	return Vector_ReadSpan(trg, src, index, 1);
}

bool Vector_WriteSpan(Vector* trg, void* src, unsigned int start, int count) {

	return trg->_type->_extensions.Transcribe((CollectionRequest) { trg, src, start, 0, count, TRANSCRIBE_RAW_TO_COLLECTION });
}
bool Vector_Write(Vector* trg, void* src, unsigned int index)
{
	return Vector_WriteSpan(trg, src, index, 1);
}

bool Vector_InsertSpan(Vector* trg, void* src, unsigned int index, unsigned int count)
{
	return trg->_type->_extensions.Modify((CollectionRequest) { trg, src, index, 0, count, MODIFY_INSERT });
}
bool Vector_Insert(Vector* trg, void* src, unsigned int index)
{
	return Vector_InsertSpan(trg, src, index, 1);
}

bool Vector_RemoveSpan(Vector* trg, void* search, unsigned int count)
{
	return trg->_type->_extensions.Modify((CollectionRequest) { trg, search, 0, 0, count, MODIFY_REMOVE_FIRST_FOUND });
}
bool Vector_Remove(Vector* trg, void* search)
{
	return Vector_RemoveSpan(trg, search, 1);
}

bool Vector_RemoveSpanAt(Vector* trg, unsigned int index, unsigned int count)
{
	return trg->_type->_extensions.Modify((CollectionRequest) { trg, 0, index, 0, count, MODIFY_REMOVE_AT });
}
bool Vector_RemoveAt(Vector* trg, unsigned int index)
{
	return Vector_RemoveSpanAt(trg, index, 1);
}

void Vector_nullTerm(Vector* null, size_t unitSize)
{

	null->_type = &NULL_TYPE;
	null->_count = 0;
	null->_bucket = NULL;
}

void Vector_ctor(
	Vector* vector,
	TypeID* type,
	unsigned int existingCount,
	unsigned int existingCapacity,
	void* existingBucket
)
{
	vector->_type = type;
	vector->_count = existingCount;
	vector->_capacity = existingCapacity;
	vector->_bucket = existingBucket;
}


void Vector_BuildTemp(char** tmp, void* head, size_t unitSize, size_t unitDelta, unsigned int count)
{
	for (int i = 0; i < count; i++)
		for (int j = 0; j < unitSize; j++)
		{
			char nextChar = ((char*)head)[(i * unitDelta) + j];
			(*tmp)[(i * unitSize) + j] = nextChar;
		}
}