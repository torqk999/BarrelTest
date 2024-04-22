#ifndef tChunk
#define tChunk
#include <tHelpers.h>

inline void* Chunk_GetPtr(Chunk* chunk, size_t unit, uint index);
Chunk Chunk_Create(void* head, size_t size);
void Chunk_ctor(Chunk* loc, void* head, size_t size);
void Chunk_Clone(Chunk* trg, Chunk* src);

#endif

