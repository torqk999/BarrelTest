#pragma once
#include <tTypeID.h>

typedef enum {
	DONE,
	ITERATE,
	HEAD,

	CREATE_NODE_BARREL,

	COMPARE_EQUIVALENCE,
	COMPARE_COMPATIBILITY_FULL,
	COMPARE_COMPATIBILITY_WRITE_ALLOWED,
	COMPARE_COMPATIBILITY_SIZE,

	//TRANSCRIBE,

	TRANSCRIBE_RAW_TO_RAW,
	TRANSCRIBE_COLLECTIONS,
	TRANSCRIBE_RAW_TO_COLLECTION,
	TRANSCRIBE_COLLECTION_TO_RAW,

	MODIFY_INSERT,
	MODIFY_REMOVE_AT,
	MODIFY_REMOVE_FIRST_FOUND,
	MODIFY_DELTA_CAPACITY,
	MODIFY_DELTA_COUNT,
	MODIFY_DECON,

	//WAIT_INSERT,
	//WAIT_REMOVE_AT,
	//WAIT_REMOVE_FIRST_FOUND,
	//WAIT_RESIZE,
	//WAIT_DECON
} RequestType;

/* Vector - Underlying functionality of the collection system, ambiguous of whether it derives
* from a 'bucket' or a 'barrel'. Includes construction, 
*/


#define PAR_COUNT(...) paramCount( ( char [] ){ __VA_ARGS__ ,  0 },( char [] ) { __VA_ARGS__ , 1 } )

#define VECTOR(vecPtr, typeName, ...) Vector_ctor(vecPtr, BUCKET, sizeof(typeName), PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_TOR(typeName, ...) Vector_Vectorize((TypeID){sizeof(typeName), #typeName}, PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_PRINT_ALL(vectorPtr, typeName, strFormat) for (int i = 0; i < ( vectorPtr ) ->_count; i++) \
{ typeName output; Vector_ReadIndex1( vectorPtr , i, &output, sizeof( typeName )); \



void foo()
{
	VEC_TOR(int, 0, 1, 2, 3, 4, 5);
}

//TypeID CollectionRequest_Type =
//{
//	sizeof()
//};

//bool Collection_Iterate(Request* request);
bool Collection_Transcribe(Collection* trg, Collection* src, unsigned int tIx, unsigned int sIx, unsigned int count);
bool Collection_Resize(Collection* trg, unsigned int count);
bool Collection_ReadSpan(Collection * src, void* trg, unsigned int start, unsigned int count);
bool Collection_Read(Collection * src, void* trg, unsigned int index);
bool Collection_WriteSpan(Collection * trg, void* src, unsigned int start, int count);
bool Collection_Write(Collection * trg, void* src, unsigned int index);
bool Collection_InsertSpan(Collection * trg, void* src, unsigned int index, unsigned int count);
bool Collection_Insert(Collection * trg, void* src, unsigned int index);
bool Collection_RemoveSpanAt(Collection * trg, unsigned int index, unsigned int count);
bool Collection_RemoveAt(Collection * trg, unsigned int index);

bool Collection_RemoveSpan(Collection* trg, void* search, unsigned int count);
bool Collection_Remove(Collection* trg, void* search);

void* Collection_Head(Collection* trg);

Collection Collection_ctor(
	TypeID* type,
	uint existingCount,
	uint existingCapacity
);

bool Collection_dtor(Collection* vector);
