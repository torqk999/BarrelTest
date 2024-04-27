#ifndef	tCollection
#define	tCollection
#include <tTypeInfo.h>
#include <tChunk.h>
//
//
///* Vector - Underlying functionality of the collection system, ambiguous of whether it derives
//* from a 'bucket' or a 'barrel'. Includes construction, 
//*/
//
////const uint GlobalClassCount = 3;
//
//bool (*Constructors)(Request request);
//
//void* Collection_Iterate(Collection* trg, Request* iter);
//void* Collection_Location(	Collection* trg, uint index);
//void Managed_Use(		ManagedCollection* trg);
//void Managed_Free(		ManagedCollection* trg);
//void Managed_Point(		ManagedCollection* trg);
//void Managed_Release(	ManagedCollection* trg);
//
////bool Collection_GetSlice(Collection* trg, Slice* slice);
//bool Collection_Transcribe(Collection* trg, Collection* src, unsigned int tIx, unsigned int sIx, unsigned int count);
//bool Collection_Resize(Collection* trg, unsigned int count);
bool Collection_Transcribe(COLLECTION trg, COLLECTION src, unsigned int trgStart, unsigned int srcStart, unsigned int count);
bool Collection_ReadSpan(COLLECTION src, void* trg, unsigned int start, unsigned int count);
bool Collection_Read(COLLECTION src, void* trg, unsigned int index);
bool Collection_WriteSpan(COLLECTION trg, void* src, unsigned int start, unsigned int count);
bool Collection_Write(COLLECTION trg, void* src, unsigned int index);
//bool Collection_InsertSpan(Collection * trg, void* src, unsigned int index, unsigned int count);
//bool Collection_Insert(Collection * trg, void* src, unsigned int index);
//bool Collection_RemoveSpanAt(Collection * trg, unsigned int index, unsigned int count);
//bool Collection_RemoveAt(Collection * trg, unsigned int index);
//
//bool Collection_RemoveSpan(Collection* trg, void* search, unsigned int count);
//bool Collection_Remove(Collection* trg, void* search);
uint Collection_Capacity(COLLECTION collection);
uint Collection_Count(COLLECTION collection);
bool Collection_ReadOnly(COLLECTION collection);
bool Collection_Compare(COLLECTION a, COLLECTION b);
bool Collection_TranscribeCompare(CollectionExtensions* A, CollectionExtensions* B);
inline bool Collection_Request(COLLECTION collection);
inline bool Collection_Release(COLLECTION collection);
CollectionExtensions CollectionExtensions_Create(TypeInfo* type, bool(*extensions)(RequestType* request), int memFlags);
Collection Collection_Create(CollectionExtensions* extensions, uint count);
CollectionExtensions* Collection_GetExtensions(TypeInfo* typeInfo, bool(*methods)(RequestType* request), int memFlags);
////Collection Collection_ctor0(TypeRaw type, ClassFlag classFlag, bool(*extensions)(Request* request), uint init);
//
//ManagedCollection Managed_ctor(Collection collection);
//
//bool Collection_dtor(Collection* vector);


#define bucket_custom(typeName, flags, ...) Bucket_ctor(#typeName, sizeof(tyeName), (Bucket){0},(typeName[]){__VA_ARGS__}, flags, PARAM_COUNT( __VA_ARGS__ ))
#define bucket(typeName, ...) bucket_custom(typeName, 0, __VA_ARGS__)

#define Bucket_List(typeName, ...) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[]){ __VA_ARGS__ }, 0, PARAM_COUNT( __VA_ARGS__ ) )
#define Bucket_Empty(typeName, capacity) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[capacity]){ 0 }, EMPTY, capacity )
#define Bucket_Fill(typeName, capacity, fill) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[capacity]){ fill }, FILL, capacity )
#define Bucket_Inject(typeName, capacity, inject) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, inject, 0, capacity )

#define Barrel_List(typeName, ...) Barrel_ctor(#typeName, sizeof(typeName), (typeName[]){ __VA_ARGS__ }, 0, PARAM_COUNT( __VA_ARGS__ ) )
#define Barrel_Empty(typeName, capacity) Barrel_ctor(#typeName, sizeof(typeName), (typeName[capacity]){ 0 }, EMPTY, capacity )
#define Barrel_Fill(typeName, capacity, fill) Barrel_ctor(#typeName, sizeof(typeName), (typeName[capacity]){ fill }, FILL, capacity )
#define Barrel_Inject(typeName, capacity, inject) Barrel_ctor(#typeName, sizeof(typeName), inject , 0, capacity )

#endif
//
//
//
//
//#define VEC_TOR(typeName, ...) Vector_Vectorize((TypeInfo){sizeof(typeName), #typeName}, PAR_COUNT(__VA_ARGS__), (typeName[]) { __VA_ARGS__ })
//
//#define VEC_PRINT_ALL(vectorPtr, typeName, strFormat) for (int i = 0; i < ( vectorPtr ) ->_count; i++) \
//{ typeName output; Vector_ReadIndex1( vectorPtr , i, &output, sizeof( typeName )); \
