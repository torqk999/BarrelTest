#ifndef tChunk
#define tChunk
#include <tHelpers.h>

bool Chunk_WriteAllowed(Chunk* chunk);
inline void* Chunk_GetPtr(Chunk* chunk, size_t unit, uint index);
Chunk Chunk_Create(void* head, size_t size, int flags);
void Chunk_ctor(Chunk* loc, void* head, size_t size, int flags);
void Chunk_Clone(Chunk* trg, Chunk* src);

#endif

