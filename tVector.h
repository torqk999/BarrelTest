#pragma once
#include <tHelpers.h>
#include <tBarrel.h>
#include <tBucket.h>

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

#define PAR_COUNT(...) paramCount( ( char [] ){ __VA_ARGS__ ,  0 },( char [] ) { __VA_ARGS__ , 1 } )

#define VECTOR(vecPtr, typeName, ...) Vector_ctor(vecPtr, BUCKET, sizeof(typeName), PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_TOR(typeName, ...) Vector_Vectorize((TypeID){sizeof(typeName), #typeName}, PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_PRINT_ALL(vectorPtr, typeName, strFormat) for (int i = 0; i < ( vectorPtr ) ->_count; i++) \
{ typeName output; Vector_ReadIndex1( vectorPtr , i, &output, sizeof( typeName )); \

//const TypeID NULL_TYPE = { 0, "NULL" };


bool Vector_Iterate(CollectionRequest* request);
bool Vector_Transcribe(CollectionRequest request);
bool Vector_Resize(Vector* trg, unsigned int count);

bool Vector_ReadSpan(Vector* src, void* trg, unsigned int start, unsigned int count);
bool Vector_Read(Vector* src, void* trg, unsigned int index);

bool Vector_WriteSpan(Vector* trg, void* src, unsigned int start, int count);
bool Vector_Write(Vector* trg, void* src, unsigned int index);

bool Vector_InsertSpan(Vector* trg, void* src, unsigned int index, unsigned int count);
bool Vector_Insert(Vector* trg, void* src, unsigned int index);

bool Vector_RemoveSpan(Vector* trg, void* search, unsigned int count);
bool Vector_Remove(Vector* trg, void* search);

bool Vector_RemoveSpanAt(Vector* trg, unsigned int index, unsigned int count);
bool Vector_RemoveAt(Vector* trg, unsigned int index);

void Vector_nullTerm(Vector* null, size_t unitSize);

void Vector_ctor(
	Vector * vector,
	TypeID * type,
	unsigned int existingCount,
	unsigned int existingCapacity,
	void* existingBucket
);


void Vector_BuildTemp(char** tmp, void* head, size_t unitSize, size_t unitDelta, unsigned int count);



//printf("[%i] : " #strFormat "\n", i, output); } \

