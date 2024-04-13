#include <tVector.h>
//
//
void Vector_Transcribe(REQUEST request)
{
	Chunk trg = *((Chunk*)request._params[tTRG]);
	Chunk src = *((Chunk*)request._params[tSRC]);

	size_t unitSize = request._params[tSIZE];
	size_t count = request._params[tCOUNT];
	size_t trgIx = request._params[Ix_TRG];
	size_t srcIx = request._params[Ix_SRC];

	size_t span = unitSize * count;
	size_t trgPtr = trg._head + (unitSize * trgIx);
	size_t srcPtr = src._head + (unitSize * trgIx);
	
	while (span)
	{
		size_t trgRemain = (trg._head + trg._size) - trgPtr;
		size_t srcRemain = (src._head + src._size) - srcPtr;
		size_t delta = trgRemain < span ? srcRemain < trgRemain ? srcRemain : trgRemain : span;
		
		if (delta >= sizeof(Barrel)) {
			uint barrelCount = delta / sizeof(Barrel);
			delta = barrelCount * sizeof(Barrel);
			
			barrelTranscribe(trgPtr, srcPtr, barrelCount);
		}
		else {
			rawTranscribe(trgPtr, srcPtr, delta);
		}

		trgPtr += delta;
		srcPtr += delta;
		span -= delta;
	}

	return true;
}