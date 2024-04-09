#include <tChunk.h>
//
//Slice Slice_ctor(
//	Collection* collection,
//	void* hLoc,
//	void* tLoc,
//	size_t hMem,
//	size_t tMem)
//{
//	return (Slice) {
//		hLoc,
//		tLoc,
//		hMem,
//		tMem
//	};
//}

bool Chunk_WriteAllowed(Chunk* chunk)
{
	return chunk->_flags & READ_ONLY;
}

inline void* Chunk_GetPtr(Chunk* chunk, size_t unit, uint index)
{
	return  (size_t)(chunk->_head) + (index * unit);
}

Chunk Chunk_Create(void* head, size_t size, int flags) {
	return (Chunk) {
			head,
			size,
			flags
	};
}

void Chunk_ctor(Chunk* loc, void* head, size_t size, int flags)
{
	Chunk tmp = Chunk_Create(head, size, flags);
	rawTranscribe(loc, &tmp, sizeof(Chunk));
}

void Chunk_Clone(Chunk* trg, Chunk* src)
{
	Chunk_ctor(trg, (size_t)(src->_head), src->_size, src->_flags);
}
