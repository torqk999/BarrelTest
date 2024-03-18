#pragma once
#include <tCollection.h>



void* Bucket_GetPtr(Bucket* bucketVector, unsigned int index);

bool Bucket_Resize(Request request);
bool Bucket_Read(Request request);
bool Bucket_Write(Request request);
bool Bucket_Add(Request request);
bool Bucket_Insert(Request request);
bool Bucket_Remove(Request request);
bool Bucket_RemoveAt(Request request);
bool Bucket_Iterate(Request* request);

bool Bucket_Head(Request* request);

bool Bucket_Extensions(Request* request);

Bucket Bucket_ctor(Collection collection, void* bucket);

#define BUCKET(typeName, flags, ...) Bucket_ctor(Collection_ctor(&((TypeID){sizeof(typeName), #typeName, flags}),)