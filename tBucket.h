#pragma once
#include <tHelpers.h>

void* Bucket_GetPtr(Vector* bucketVector, unsigned int index);

bool Bucket_ReadFromVector(CollectionRequest request);
bool Bucket_WriteToVector(CollectionRequest request);
bool Bucket_Insert(CollectionRequest request);
bool Bucket_Remove(CollectionRequest request);
bool Bucket_RemoveAt(CollectionRequest request);

bool Bucket_Iterate(CollectionRequest* request);
bool Bucket_Transcribe(CollectionRequest request);
bool Bucket_Resize(CollectionRequest request);
bool Bucket_Modify(CollectionRequest request);

CollectionExtensions Bucket_TemplateExtension();
