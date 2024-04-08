#ifndef	tBucket
#define	tBucket

#include <tCollection.h>

void* Bucket_GetPtr(Bucket* bucketVector, uint index);

bool Bucket_Resize(Request request);
bool Bucket_Read(Request request);
bool Bucket_Write(Request request);
bool Bucket_Add(Request request);
bool Bucket_Insert(Request request);
bool Bucket_Remove(Request request);
bool Bucket_RemoveAt(Request request);

bool Bucket_Slice(Request request);

bool Bucket_Location(Request* request);
bool Bucket_Iterate(Request* request);

bool Bucket_Extensions(Request* request);

Bucket Bucket_ctor(Collection collection, void* bucket);

COLLECTION Bucket_Construct(size_t size, const char* typeName, void* src);

#define bucket(typeName, ...) Bucket_Construct(sizeof(typeName), #typeName, (ullong[]){0,0,PARAM_COUNT(__VA_ARGS__),(typeName[]){__VA_ARGS__}})

void foo()
{
	COLLECTION myBucket = bucket(int, 0, 1, 2, 3, 4);

	int myArray[] = {0,1,2,3,4};
}

#endif