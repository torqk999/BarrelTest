#ifndef	tBucket
#define	tBucket

#include <tVector.h>

//void* Bucket_GetPtr(Bucket* bucketVector, uint index);
//
//bool Bucket_Resize(REQUEST request);
//bool Bucket_Read(REQUEST request);
//bool Bucket_Write(REQUEST request);
//bool Bucket_Add(REQUEST request);
//bool Bucket_Insert(REQUEST request);
//bool Bucket_Remove(REQUEST request);
//bool Bucket_RemoveAt(REQUEST request);
//
//bool Bucket_Chunk(REQUEST request);
//
//bool Bucket_Location(REQUEST* request);
//bool Bucket_Iterate(REQUEST* request);

bool Bucket_Extensions(REQUEST request);

COLLECTION Bucket_ctor(const char* name, size_t unitSize, void* loc, void* src, int memFlags, uint count);

#define bucket_custom(typeName, flags, ...) Bucket_ctor(#typeName, sizeof(tyeName), (Bucket){0},(typeName[]){__VA_ARGS__}, flags, PARAM_COUNT( __VA_ARGS__ ))
#define bucket(typeName, ...) bucket_custom(typeName, 0, __VA_ARGS__)

#define Bucket_List(typeName, ...) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[]){ __VA_ARGS__ }, 0, PARAM_COUNT( __VA_ARGS__ ) )
#define Bucket_Empty(typeName, capacity) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[capacity]){ 0 }, EMPTY, capacity )

#endif