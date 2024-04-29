#ifndef	tCollection
#define	tCollection
#include <tTypeInfo.h>
#include <tChunk.h>

bool Collection_Compare(CollectionExtensions* a, CollectionExtensions* b);
bool Collection_TranscribeCompare(CollectionExtensions* A, CollectionExtensions* B);
CollectionExtensions CollectionExtensions_Create(TypeInfo* type, bool(*extensions)(RequestType* request), int memFlags);
Collection Collection_Create(CollectionExtensions* extensions, uint count);
CollectionExtensions* Collection_GetExtensions(TypeInfo* typeInfo, bool(*methods)(RequestType* request), int memFlags);



#endif
//
//
//
//
//#define VEC_TOR(typeName, ...) Vector_Vectorize((TypeInfo){sizeof(typeName), #typeName}, PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })
//
//#define VEC_PRINT_ALL(vectorPtr, typeName, strFormat) for (int i = 0; i < ( vectorPtr ) ->_count; i++) \
//{ typeName output; Vector_ReadIndex1( vectorPtr , i, &output, sizeof( typeName )); \
