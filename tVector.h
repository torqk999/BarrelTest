#pragma once
#include <tModules.h>

LinkedVector* LinkedVector_GetTermNode(LinkedVector* vector)
{
	LinkedVector* linked = (LinkedVector*)(vector->_link._next);

	while (linked)
		linked = (LinkedVector*)(linked->_link._next);

	return vector;
}

long long Vector_GetIndex(Vector* vector, void* trg)
{
	return ((long long)trg - (long long)(vector->_bucket)) / vector->_type->_size;
}

void* Vector_GetPtr(Vector* vector, unsigned int index)
{
	if (index < 0 || index > vector->_count)
		return NULL;

	return &(((char*)(vector->_bucket))[vector->_type->_size * index]);
}

unsigned int LinkedVector_GetCount(LinkedVector* vector)
{
	unsigned int _output = 0;

	_output += vector->_vector._count;

	if (vector->_link._next)
		_output += LinkedVector_GetCount((LinkedVector*)(vector->_link._next));

	return _output;
}

void* Vector_Iterate(Vector* vector, void* current, int delta)
{
	size_t trg = (size_t)current + (delta * vector->_type->_size);
	size_t start = (size_t)(vector->_bucket);

	if (trg < start || trg >= start + ((vector->_count - 1) * vector->_type->_size))
		return NULL;
	
	return (void*)trg;
}

void* LinkedVector_Iterate(LinkedVector** vector, void* current, int delta)
{
	size_t trg = (size_t)current + (delta * (*vector)->_vector._type->_size);
	size_t start = (size_t)((*vector)->_vector._bucket);
	long long index = Vector_GetIndex(&((*vector)->_vector), (void*)trg);


	if (index < 0)
	{
		if (!(*vector)->_link._prev)
			return NULL;
		*vector = (LinkedVector*)((*vector)->_link._prev);
		index += (*vector)->_vector._count;
		trg = (size_t)((*vector)->_vector._bucket) + (index * (*vector)->_vector._type->_size);
		return LinkedVector_Iterate(vector, (void*)trg, 0);
	}

	if (trg >= start + (((*vector)->_vector._count) * (*vector)->_vector._type->_size))
	{
		if (!(*vector)->_link._next)
			return NULL;
		*vector = (LinkedVector*)((*vector)->_link._next);
		index -= (*vector)->_vector._count;
		trg = (size_t)((*vector)->_vector._bucket) + (index * (*vector)->_vector._type->_size);
		return LinkedVector_Iterate(vector, (void*)trg, 0);
	}

	return (void*)trg;
}

void Vector_MoveIndex(void* src, void* trg, unsigned int srcIndex, unsigned int trgIndex, size_t unitSize)
{
	for (int i = 0; i < unitSize; i++)
	{
		char next = ((char*)src)[(unitSize * srcIndex) + i];
		((char*)trg)[(unitSize * trgIndex) + i] = next;
	}
}

bool Vector_Write1(Vector* vector, void* src, unsigned int start, unsigned int count)
{
	//if (vector->_type._flags == READ_ONLY)
	//	return false;

	if (start < 0 || start >= vector->_count ||
		start + count < 0 || start + count >= vector->_count)
		return false;

	unsigned int newCount = start + count;

	for (int i = 0; i < count; i++)
		Vector_MoveIndex(src, vector->_bucket, i, i + start, vector->_type->_size);

	vector->_count = newCount;

	return true;
}

bool Vector_Read(Vector* vector, void* trg, unsigned int start, unsigned int count)
{
	if (start < 0 || start >= vector->_count)
		return false;

	if (start + count >= vector->_count)
		return false;

	for (int i = 0; i < count; i++)
		Vector_MoveIndex(vector->_bucket, trg, i + start, i, vector->_type->_size);
	return true;
}

bool Vector_WriteIndex(Vector* vector, void* src, unsigned int vecIndex)
{
	if (vecIndex < 0 || vecIndex >= vector->_count)
		return false;

	Vector_MoveIndex(src, vector->_bucket, 0, vecIndex, vector->_type->_size);
	return true;
}

bool Vector_ReadIndex(Vector* vector, void* trg, unsigned int vecIndex)
{
	if (vecIndex < 0 || vecIndex >= vector->_count)
		return false;

	Vector_MoveIndex(vector->_bucket, trg, vecIndex, 0, vector->_type->_size);
	return true;
}

bool Vector_Transcribe1(Vector* trg, Vector* src, unsigned int trgIndex, unsigned int srcIndex)
{
	if (src->_type->_size != trg->_type->_size)
		return false;

	if (srcIndex < 0 || srcIndex >= src->_count ||
		trgIndex < 0 || trgIndex >= trg->_count)
		return false;

	Vector_MoveIndex(src->_bucket, trg->_bucket, srcIndex, trgIndex, src->_type->_size);
	return true;
}

bool Vector_Transcribe0(Vector* trg, Vector* src, unsigned int index)
{
	return Vector_Transcribe1(trg, src, index, index);
}

const TypeID null = { 0, "NULL" };

void Vector_nullTerm(
	Vector* null,
	size_t unitSize)
{
	
	null->_type = &null;
	null->_count = 0;
	null->_bucket = NULL;
}

void Vector_ctor(
	Vector* vector,
	TypeID type,
	unsigned int existingCount,
	void* existingBucket
	)
{
	vector->_type = &type;
	vector->_count = existingCount;
	vector->_bucket = existingBucket;
}

void LinkedVector_ctor(
	LinkedVector* linked,
	Link* next,
	Link* prev,
	TypeID type,
	unsigned int existingCount,
	void* existingBucket
)
{
	Vector_ctor(&(linked->_vector), type, existingCount, existingBucket);

	linked->_link._next = next;
	linked->_link._prev = prev;
}

LinkedVector Vector_Vectorize(TypeID type, unsigned int existingCount, void* existingBucket)
{
	LinkedVector vector;

	LinkedVector_ctor(&vector, NULL, NULL, type, existingCount, existingBucket);

	return vector;
}

void Vector_Link(LinkedVector* trg, LinkedVector* link)
{
	if (trg)
		trg->_link._next = link ? trg->_vector._type->_size == link->_vector._type->_size ? &(link->_link) : NULL : NULL;

	if (link)
		link->_link._prev = trg ? trg->_vector._type->_size == link->_vector._type->_size ? &(trg->_link) : NULL : NULL;
}

bool Vector_Insert(Vector* trg, unsigned int index, void* src)
{
	for (int i = trg->_count; i > index; i--)
		Vector_Transcribe1(trg, trg, i, i - 1);

	Vector_WriteIndex(trg, src, index);
	trg->_count++;
	return true;
}

bool Vector_Remove(Vector* vec, void* trg)
{
	unsigned long long index = ((unsigned long long)trg - (unsigned long long)(vec->_bucket)) / (unsigned long long)(vec->_type->_size);
	//printf("target removal index: %u\n", index);
	Vector_RemoveAt(vec, index);
}

bool Vector_RemoveAt(Vector* trg, unsigned int index)
{
	for (int i = index; i < trg->_count; i++)
		Vector_Transcribe1(trg, trg, i, i + 1);

	trg->_count--;
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

#define PAR_COUNT(...) paramCount( ( char [] ){ __VA_ARGS__ ,  0 },( char [] ) { __VA_ARGS__ , 1 } )

#define VECTOR(vecPtr, typeName, ...) Vector_ctor(vecPtr, STACK_VEC, sizeof(typeName), PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_TOR(typeName, ...) Vector_Vectorize((TypeID){sizeof(typeName), #typeName}, PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_PRINT_ALL(vectorPtr, typeName, strFormat) for (int i = 0; i < ( vectorPtr ) ->_count; i++) \
{ typeName output; Vector_ReadIndex1( vectorPtr , i, &output, sizeof( typeName )); \

//printf("[%i] : " #strFormat "\n", i, output); } \

