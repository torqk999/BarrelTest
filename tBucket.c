#include <tBucket.h>
//
inline void* Bucket_GetPtr(Bucket* bucket, uint index)
{
	return index >= bucket->_count ? NULL : (size_t)(bucket->_chunk._head) + (index * bucket->_collection._type->_size);
}
//
//bool Bucket_Resize(Request request)
//{
//	Bucket* bucketVector = request._trg;
//	if (request._size > bucketVector->_collection._capacity)
//		return false;
//
//	bucketVector->_collection._count = request._size;
//	return true;
//}
//bool Bucket_Write(Request request) {
//
//	Bucket* trg = request._trg;
//
//	if (trg->_collection._info->_flags & READ_ONLY)
//		return false;
//
//	if (request._trgIx + request._size >= trg->_collection._capacity)
//		return false;
//
//	request._trg = trg->_bucket;
//	if (request._type & TRANSCRIBE_COLLECTIONS)
//		request._src = ((Bucket*)request._src)->_bucket;
//	request._srcIx = 0;
//
//	TranscribeSpan(request);
//
//	return true;
//}
//bool Bucket_Read(Request request) {
//
//	Bucket* src = request._src;
//
//	if (request._srcIx + request._size >= src->_collection._count)
//		return false;
//
//	request._src = src->_bucket;
//	if (request._type & TRANSCRIBE_COLLECTIONS)
//		request._trg = ((Bucket*)request._trg)->_bucket;
//	request._trgIx = 0;
//
//	TranscribeSpan(request);
//
//	return true;
//}
//bool Bucket_Add(Request request) {}
//bool Bucket_Insert(Request request)
//{
//	Bucket* trg = request._trg;
//
//	if (trg->_collection._count >= trg->_collection._capacity)
//	{
//		trg->_collection._count = trg->_collection._capacity;
//		return false;
//	}
//
//
//	void* insertion = request._src;
//	int target = request._trgIx;
//
//	request._src = trg->_bucket;
//	request._srcIx = trg->_collection._count - 1;
//	request._trgIx = trg->_collection._count + request._size - 1;
//	request._size *= -1;
//
//	Bucket_Write(request);
//
//	request._src = insertion;
//	request._srcIx = 0;
//	request._trgIx = target;
//	request._size *= 1;
//
//	Bucket_Write(request);
//
//	trg->_collection._count++;
//	return true;
//}
//bool Bucket_Remove(Request request)
//{
//
//}
//bool Bucket_RemoveAt(Request request)
//{
//
//}
//bool Bucket_Location(Request* request) {
//	Bucket* bucket = request->_src;
//	if (request->_srcIx >= bucket->_collection._capacity)
//		return false;
//	request->_trg = ((char*)(bucket->_bucket))[request->_srcIx * bucket->_collection._info->_type._size];
//	return true;
//}
//bool Bucket_Slice(Request request)
//{
//	Bucket* bucket = request._trg;
//	Slice* slice = request._src;
//
//	size_t unitSize = bucket->_collection._info->_type._size;
//	size_t headSize = (bucket->_collection._count - request._trgIx) * unitSize;
//	size_t requestSize = request._size * unitSize;
//	
//	slice->_hLoc = Bucket_GetPtr(bucket, request._trgIx);
//	slice->_tLoc = bucket->_bucket;//Bucket_GetPtr(bucket, 0);
//	slice->_hMem = headSize < requestSize ? headSize : requestSize;
//	slice->_tMem = headSize < requestSize ? requestSize - headSize: 0;
//
//	return true;
//}
//bool Bucket_Iterate(Request* request)
//{
//	Bucket* bucketVector = request->_trg;
//	unsigned int* current = &(request->_trgIx);
//	unsigned int* count = &(request->_size);
//
//	if (*current >= bucketVector->_collection._capacity)
//		return false;
//
//	void* nxt = Bucket_GetPtr(bucketVector, *current);
//
//	*current = (*current) + 1;
//
//	*count = *current == *count ? 0 : *count;
//
//	return nxt;
//}
//

uint Bucket_Capacity(Bucket* bucket) { return (bucket->_chunk._size) / bucket->_collection._type->_size; }

inline bool Bucket_Manage(REQUEST request) {
	//Bucket* bucket = request._params[0];
	//void* output = request._params[1];
	//ParamType option = (ParamType)request._params[2];

	switch (request._type)
	{
	case tCHUNK:;
		request._params[tSRC] = &(((Bucket*)request._params[tTRG])->_chunk);
		return true;

	default:
		break;
	}
}

inline bool Bucket_Transcribe(REQUEST request) {

	ParamType dir = (ParamType)request._params[tDIRECTION];
	ParamType var = (ParamType)request._params[tVARIANT];

	ParamType bucketIx = dir == tWRITE ? tSRC : tTRG;
	ParamType aggrIx = dir == tWRITE ? tSRC : tTRG;

	Bucket* bucket = request._params[bucketIx];
	request._params[tSIZE] = bucket->_collection._type->_size;

	Chunk aggr;

	switch (var) {
	case tRAW:
		Chunk_ctor(&aggr, request._params[aggrIx], (size_t)request._params[tSIZE] * (size_t)request._params[tCOUNT], 0);
		break;

	case tCOLLECTION:;
		Collection* collection = request._params[aggrIx];
		if (collection->_type->_size != request._params[tSIZE])
			return false;
		if (!collection->_extensions(Request(MANAGE, P_(tVARIANT, tCHUNK), P_(tSRC, collection), P_(tTRG, &aggr))))
			return false;
		break;
	default:
		return false;
	}

	request._params[bucketIx] = &(bucket->_chunk);
	request._params[aggrIx] = &aggr;
	Vector_Transcribe(request);

	return true;
}

inline bool Bucket_Info(REQUEST request) {

	Bucket* bucket = request._params[tTRG];

	uint paramIndex = 0;
	while (request._params[paramIndex] != NULL) {
		void* next = request._params[paramIndex];
		switch (paramIndex) {

		case tCOUNT:
			*((uint*)next) = bucket->_count;
			break;

		case tCAPACITY:
			*((uint*)next) = Bucket_Capacity(bucket);
			break;

		case tNAME:
		case tSIZE:
			request._params[tVARIANT] = paramIndex;
			if (!TypeInfo_GetInfo(request))
				return false;
			break;

		default:
			break;
		}
		paramIndex++;
	}
}

bool Bucket_Extensions(REQUEST request)
{
	switch (request._type) {

	case INFO:
		return Bucket_Info(request);

	case TRANSCRIBE:
		return Bucket_Transcribe(request);

	case MANAGE:
		return Bucket_Manage(request);

		//case LOCATION:
		//	return Bucket_Location(request);
		//
		//case SLICE_CREATE:
		//	return Bucket_Slice(*request);
		//
		//case MODIFY_DELTA_CAPACITY:
		//	return Bucket_Resize(*request);
		//
		//case TRANSCRIBE_RAW_TO_COLLECTION:
		//case TRANSCRIBE_COLLECTIONS:
		//	return Bucket_Write(*request);
		//
		//case TRANSCRIBE_COLLECTION_TO_RAW:
		//	return Bucket_Read(*request);
		//
		//case MODIFY_INSERT:
		//	return Bucket_Insert(*request);
		//
		//case MODIFY_REMOVE_FIRST_FOUND:
		//	return Bucket_Remove(*request);
		//
		//case MODIFY_REMOVE_AT:
		//	return Bucket_RemoveAt(*request);
		//
		//case ITERATE:
		//	return Bucket_Iterate(request);

	default:
		return false;
	}
}

COLLECTION Bucket_ctor(const char* name, size_t unitSize, void* loc, void* src, int memFlags, uint count) {

	TypeInfo* info = TypeInfo_Get(name, unitSize);
	Bucket tmpBucket = {
		Collection_ctor(info, Bucket_Extensions),
		Chunk_Create(src, info->_size * count, memFlags)
	};
	rawTranscribe(loc, &tmpBucket, sizeof(Bucket));
	Bucket* returnBucket = loc;
	return returnBucket;
}



