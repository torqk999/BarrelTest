#ifndef	tBucket
#define	tBucket

#include <tCollection.h>
#include <tChunk.h>

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

//bool Bucket_Extensions(REQUEST request);

Bucket Bucket_Create(const char* name, size_t unitSize, void* head, int memFlags, uint capacity);

COLLECTION Bucket_ctor(const char* name, size_t unitSize, void* loc, void* src, int memFlags, uint count);


#endif