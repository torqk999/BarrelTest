#include <tSlice.h>

Slice Slice_ctor(
	Collection* collection,
	void* hLoc,
	void* tLoc,
	size_t hMem,
	size_t tMem)
{
	return (Slice) {
		hLoc,
		tLoc,
		hMem,
		tMem
	};
}
