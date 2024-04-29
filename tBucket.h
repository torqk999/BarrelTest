#ifndef	tBucket
#define	tBucket

#include <tCollection.h>
#include <tChunk.h>

inline void* Bucket_GetPtr(Bucket* bucket, uint index);
bool Bucket_GetChunk(Bucket* bucket, Chunk* loc);
uint Bucket_Capacity(Bucket* bucket);
inline bool Bucket_Iterate(REQUEST request);
inline bool Bucket_Manage(REQUEST request);
inline bool Bucket_Transcribe(REQUEST request);
inline bool Bucket_Info(REQUEST request);
bool Bucket_Methods(REQUEST request);

Bucket Bucket_Create(const char* name, size_t unitSize, void* head, int memFlags, uint capacity);

#endif