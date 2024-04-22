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
	size_t trgPtr = (unitSize * trgIx);
	size_t srcPtr = src._head + (unitSize * srcIx);
	
	for (size_t i = 0; i < span; i++)
		*((char*)(((trgPtr + i) % trg._size) + trg._head)) = *((char*)(((srcPtr + i) % src._size) + src._head));

	//while (span)
	//{
	//	
	//
	//
	//	size_t trgRemain = (trg._head + trg._size) - trgPtr;
	//	trgPtr = trgRemain == 0 ? trg._head : trgPtr;
	//	trgRemain = trgRemain == 0 ? trg._size : trgRemain;
	//
	//	size_t srcRemain = (src._head + src._size) - srcPtr;
	//	srcPtr = srcRemain == 0 ? src._head : srcPtr;
	//	srcRemain = srcRemain == 0 ? src._size : srcRemain;
	//
	//	size_t delta = trgRemain < span ? srcRemain < trgRemain ? srcRemain : trgRemain : span;
	//	
	//	//if (delta >= sizeof(Barrel)) {
	//	//	
	//	//	uint barrelCount = delta / sizeof(Barrel);
	//	//	delta = barrelCount * sizeof(Barrel);
	//	//	
	//	//	barrelTranscribe(trgPtr, srcPtr, barrelCount);
	//	//}
	//	//else {
	//	rawTranscribe(trgPtr, srcPtr, delta);
	//	//}
	//
	//	trgPtr += delta;
	//	srcPtr += delta;
	//	span -= delta;
	//}

}

bool Vector_Transcribe0(REQUEST request) {

	ParamType dir = (ParamType)request._params[tDIRECTION];
	ParamType var = (ParamType)request._params[tVARIANT];

	ParamType hostIx = dir == tWRITE ? tTRG : tSRC;
	ParamType aggrIx = dir == tWRITE ? tSRC : tTRG;

	COLLECTION hostCol = request._params[hostIx];
	COLLECTION aggrCol = NULL;

	Chunk host;
	Chunk aggr; // Aggregate from the requested collection source.

	size_t size = hostCol->_extensions->_type->_size;
	size_t count = (size_t)request._params[tCOUNT];

	request._params[tSIZE] = size;

	if (!hostCol->_extensions->_methods(Request(MANAGE, P_(tVARIANT, tCHUNK), P_(tSRC, hostCol), P_(tTRG, &host))))
		return false;


	switch (var) {
	case tRAW:
		Chunk_ctor(&aggr, request._params[aggrIx], (size_t)request._params[tSIZE] * count);
		break;

	case tCOLLECTION:;
		aggrCol = request._params[aggrIx];
		if (aggrCol->_extensions->_type->_size != request._params[tSIZE])
			return false;
		if (!aggrCol->_extensions->_methods(Request(MANAGE, P_(tVARIANT, tCHUNK), P_(tSRC, aggrCol), P_(tTRG, &aggr))))
			return false;
		break;
	default:
		return false;
	}

	request._params[hostIx] = &host;
	request._params[aggrIx] = &aggr;
	Vector_Transcribe0(request);


	return true;
}