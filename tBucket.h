#pragma once
#include <tHelpers.h>

void* Bucket_GetPtr(Vector* bucketVector, unsigned int index)
{
	return index >= bucketVector->_count ? NULL : (size_t)(bucketVector->_bucket) + (index * bucketVector->_type->_size);
}

bool Bucket_Resize(CollectionRequest request)
{
	Vector* bucketVector = request._trg;
	if (request._count > bucketVector->_capacity)
		return false;

	bucketVector->_count = request._count;
	return true;
}

bool Bucket_WriteToVector(CollectionRequest request) {

	Vector* trg = request._trg;

	if (trg->_type->_flags & READ_ONLY)
		return false;

	if (request._trgIx + request._count >= trg->_count)
		return false;

	request._trg = trg->_bucket;
	request._srcIx = 0;

	TranscribeSpan(request);

	return true;
}

bool Bucket_ReadFromVector(CollectionRequest request) {

	Vector* src = request._src;

	if (request._srcIx + request._count >= src->_count)
		return false;

	request._src = src->_bucket;
	request._trgIx = 0;

	TranscribeSpan(request);

	return true;
}

bool Bucket_Insert(CollectionRequest request)
{
	Vector* trg = request._trg;

	if (trg->_count >= trg->_capacity)
	{
		trg->_count = trg->_capacity;
		return false;
	}

	void* insertion = request._src;
	int target = request._trgIx;

	request._src = trg->_bucket;
	request._trg = trg->_bucket;
	request._srcIx = trg->_count - 1;
	request._trgIx = trg->_count + request._count - 1;

	for (int i = trg->_count + request._count - 1; i > request._trgIx; i--) {
		Bucket_TranscribeElement(request);
		request._srcIx--;
		request._trgIx--;
	}

	request._src = insertion;
	request._trgIx = target;
	request._srcIx = 0;

	for (int i = 0; i < request._count; i++) {
		Bucket_TranscribeElement(request);
		request._srcIx++;
		request._trgIx++;
	}

	trg->_count++;
	return true;
}

void* Bucket_Iterate(CollectionRequest* request)
{
	Vector* bucketVector = request->_src;
	unsigned int* current = &(request->_trgIx);
	unsigned int* count = &(request->_count);

	if (*current >= bucketVector->_capacity)
		return NULL;

	void* nxt = Bucket_GetPtr(bucketVector, *current);

	*current += *count;
	*count = *count - 1;

	return nxt;
}

bool Bucket_Transcribe(CollectionRequest request)
{
	switch (request._type)
	{
	case TRANSCRIBE_RAW_TO_COLLECTION:
		return Bucket_WriteToVector(request);

	case TRANSCRIBE_COLLECTION_TO_RAW:
		return Bucket_ReadFromVector(request);

	default:
		return false;
	}
}

bool Bucket_Modify(CollectionRequest request)
{
	switch (request._type)
	{
	case MODIFY_INSERT:
		return Bucket_Insert(request);

	case MODIFY_RESIZE:
		return Bucket_Resize(request);

	default:
		return false;
	}
}