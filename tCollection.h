#ifndef	tCollection
#define	tCollection
#include <tRequest.h>
#include <tTypeInfo.h>


/* Vector - Underlying functionality of the collection system, ambiguous of whether it derives
* from a 'bucket' or a 'barrel'. Includes construction, 
*/

const uint GlobalClassCount = 3;

bool (*Constructors)(Request request);

void* Collection_Iterate(Collection* trg, Request* iter);
void* Collection_Head(	Collection* trg);
void Managed_Use(		ManagedCollection* trg);
void Managed_Free(		ManagedCollection* trg);
void Managed_Point(		ManagedCollection* trg);
void Managed_Release(	ManagedCollection* trg);

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

Collection Collection_ctor(TypeInfo* info, bool(*extensions)(Request* request), uint init);
Collection Collection_ctor0(TypeRaw type, ClassFlag classFlag, bool(*extensions)(Request* request), uint init);

ManagedCollection Managed_ctor(Collection collection);

bool Collection_dtor(Collection* vector);

#endif




#define VEC_TOR(typeName, ...) Vector_Vectorize((TypeInfo){sizeof(typeName), #typeName}, PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })

#define VEC_PRINT_ALL(vectorPtr, typeName, strFormat) for (int i = 0; i < ( vectorPtr ) ->_count; i++) \
{ typeName output; Vector_ReadIndex1( vectorPtr , i, &output, sizeof( typeName )); \
