#include <tBucket.h>



void* Bucket_GetPtr(Bucket* bucket, unsigned int index)
{
	return index >= bucket->_collection._count ? NULL : (size_t)(bucket->_bucket) + (index * bucket->_collection._type->_size);
}

bool Bucket_Resize(Request request)
{
	Bucket* bucketVector = request._trg;
	if (request._count > bucketVector->_collection._capacity)
		return false;

	bucketVector->_collection._count = request._count;
	return true;
}
bool Bucket_Read(Request request) {

	Bucket* src = request._src;

	if (request._srcIx + request._count >= src->_collection._count)
		return false;

	request._src = src->_bucket;
	request._trgIx = 0;

	TranscribeSpan(request);

	return true;
}
bool Bucket_Write(Request request) {

	Bucket* trg = request._trg;

	if (trg->_collection._type->_flags & READ_ONLY)
		return false;

	if (request._trgIx + request._count >= trg->_collection._capacity)
		return false;

	request._trg = trg->_bucket;
	request._srcIx = 0;

	TranscribeSpan(request);

	return true;
}
bool Bucket_Add(Request request) {}
bool Bucket_Insert(Request request)
{
	Bucket* trg = request._trg;

	if (trg->_collection._count >= trg->_collection._capacity)
	{
		trg->_collection._count = trg->_collection._capacity;
		return false;
	}


	void* insertion = request._src;
	int target = request._trgIx;

	request._src = trg->_bucket;
	request._srcIx = trg->_collection._count - 1;
	request._trgIx = trg->_collection._count + request._count - 1;
	request._count *= -1;

	Bucket_Write(request);

	request._src = insertion;
	request._srcIx = 0;
	request._trgIx = target;
	request._count *= 1;

	Bucket_Write(request);

	trg->_collection._count++;
	return true;
}
bool Bucket_Remove(Request request)
{

}
bool Bucket_RemoveAt(Request request)
{

}

bool Bucket_Iterate(Request* request)
{
	Bucket* bucketVector = request->_trg;
	unsigned int* current = &(request->_trgIx);
	unsigned int* count = &(request->_count);

	if (*current >= bucketVector->_collection._capacity)
		return false;



	void* nxt = Bucket_GetPtr(bucketVector, *current);

	*current = (*current) + 1;

	*count = *current == *count ? 0 : *count;

	

	return nxt;
}
bool Bucket_Transcribe(Request request)
{
	switch (request._type)
	{
	case TRANSCRIBE_RAW_TO_COLLECTION:
		return Bucket_Write(request);

	case TRANSCRIBE_COLLECTION_TO_RAW:
		return Bucket_Read(request);

	default:
		return false;
	}
}
bool Bucket_Modify(Request request)
{
	switch (request._type)
	{
	case MODIFY_INSERT:
		return Bucket_Insert(request);

	case MODIFY_REMOVE_AT:
		return Bucket_RemoveAt(request);

	case MODIFY_REMOVE_FIRST_FOUND:
		return Bucket_Remove(request);

	case MODIFY_RESIZE:
		return Bucket_Resize(request);

	default:
		return false;
	}
}

CollectionExtensions Bucket_TemplateExtensions =
{
	Bucket_Iterate,
	Bucket_Transcribe,
	Bucket_Modify
};

Bucket Bucket_ctor(Collection collection, void* bucket) {
	Bucket newBucket;

	newBucket._collection = collection;
	newBucket._bucket = bucket;

	return newBucket;
}

