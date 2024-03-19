#include <tBucket.h>

void* Bucket_GetPtr(Bucket* bucket, unsigned int index)
{
	return index >= bucket->_collection._count ? NULL : (size_t)(bucket->_bucket) + (index * bucket->_collection._info->_type._size);
}


bool Bucket_Resize(Request request)
{
	Bucket* bucketVector = request._trg;
	if (request._count > bucketVector->_collection._capacity)
		return false;

	bucketVector->_collection._count = request._count;
	return true;
}
bool Bucket_Write(Request request) {

	Bucket* trg = request._trg;

	if (trg->_collection._info->_flags & READ_ONLY)
		return false;

	if (request._trgIx + request._count >= trg->_collection._capacity)
		return false;

	request._trg = trg->_bucket;
	if (request._type & TRANSCRIBE_COLLECTIONS)
		request._src = ((Bucket*)request._src)->_bucket;
	request._srcIx = 0;

	TranscribeSpan(request);

	return true;
}
bool Bucket_Read(Request request) {

	Bucket* src = request._src;

	if (request._srcIx + request._count >= src->_collection._count)
		return false;

	request._src = src->_bucket;
	if (request._type & TRANSCRIBE_COLLECTIONS)
		request._trg = ((Bucket*)request._trg)->_bucket;
	request._trgIx = 0;

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
bool Bucket_Head(Request request) {
	*((void**)(request._trg)) = ((Bucket*)(request._src))->_bucket;
	return true;
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

bool Bucket_Extensions(Request* request)
{
	switch (request->_type)
	{


	case HEAD:
		return Bucket_Head(*request);



	case MODIFY_DELTA_CAPACITY:
		return Bucket_Resize(*request);

	case TRANSCRIBE_RAW_TO_COLLECTION:
	case TRANSCRIBE_COLLECTIONS:
		return Bucket_Write(*request);

	case TRANSCRIBE_COLLECTION_TO_RAW:
		return Bucket_Read(*request);

	case MODIFY_INSERT:
		return Bucket_Insert(*request);

	case MODIFY_REMOVE_FIRST_FOUND:
		return Bucket_Remove(*request);

	case MODIFY_REMOVE_AT:
		return Bucket_RemoveAt(*request);

	case ITERATE:
		return Bucket_Iterate(request);

	default:
		return false;
	}
}

Bucket Bucket_ctor(Collection collection, void* bucket) {
	Bucket newBucket;

	newBucket._collection = collection;
	newBucket._collection._extensions = Bucket_Extensions;
	newBucket._bucket = bucket;

	return newBucket;
}

COLLECTION Bucket_Construct(size_t size, const char* typeName, void* src)
{
	Bucket* bucket = src;
	bucket->_collection = Collection_ctor0(Type_CreateRaw(size, typeName), BUCKET, Bucket_Extensions, bucket->_collection._count);
	return bucket;
}


