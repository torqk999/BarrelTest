#include <tVector.h>


bool Vector_Transcribe(Request request)
{
	Collection* trgCollection = request._trg;
	Collection* srcCollection = request._src;

	request._type = COMPARE_COMPATIBILITY_FULL;

	if (!TypeID_Compare(request))
		return false;

	Slice trgSlice, srcSlice;
	Request trgRequest = { SLICE_CREATE, trgCollection, &trgSlice, request._trgIx, 0, request._size };
	Request srcRequest = { SLICE_CREATE, srcCollection, &srcSlice, request._srcIx, 0, request._size };

	if (!trgCollection->_extensions(&trgRequest) ||
		!srcCollection->_extensions(&srcRequest))
		return false;

	size_t trgRemain = trgSlice._hMem;
	size_t srcRemain = srcSlice._hMem;
	
	void* trgLoc = trgSlice._hLoc;
	void* srcLoc = srcSlice._hLoc;

	while (srcRemain || trgRemain)
	{
		size_t delta = trgRemain < srcRemain ? trgRemain : srcRemain;
		
		if (delta >= sizeof(Barrel)) {
			uint barrelCount = delta / sizeof(Barrel);
			delta = barrelCount * sizeof(Barrel);
			
			barrelTranscribe(trgLoc, srcLoc, barrelCount);
		}
		else {
			rawTranscribe(trgLoc, srcLoc, delta);
		}

		trgRemain -= delta;
		srcRemain -= delta;
		
		if (!trgRemain && trgSlice._tMem) {
			trgRemain = trgSlice._tMem;
			trgLoc = trgSlice._tLoc;
		}

		if (!srcRemain && srcSlice._tMem) {
			srcRemain = srcSlice._tMem;
			srcLoc = srcSlice._tLoc;
		}
	}
	
	trgRequest._type = SLICE_RELEASE;
	srcRequest._type = SLICE_RELEASE;

	trgCollection->_extensions(&trgRequest);
	srcCollection->_extensions(&srcRequest);

	return true;
}