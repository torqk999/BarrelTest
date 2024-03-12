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
bool Bucket_Transcribe(Request request);
bool Bucket_Modify(Request request);

CollectionExtensions Bucket_TemplateExtensions;
//{
//	Bucket_Iterate,
//	Bucket_Transcribe,
//	Bucket_Modify
//};

Bucket Bucket_ctor(Collection collection, void* bucket);
