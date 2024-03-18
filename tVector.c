#include <tVector.h>

void Vector_TranscribeElement(Request request)
{
	Collection* src = request._src;
	Collection* trg = request._trg;



	while (targetNode->_flags & req_ROLL ||
		targetNode->_flags & req_FREE) { /* Wait for lock to free */
	}

	InterlockedIncrement(&(targetNode->_userCount));

	void* barrelPtr = barrel_GetElementLocation(targetNode, index);
	void* readPtr = isRead ? barrelPtr : target;
	void* writePtr = isRead ? target : barrelPtr;

	rawTranscribe(readPtr, writePtr, targetNode->_collection._type->_size);

	InterlockedDecrement(&(targetNode->_userCount));
}
void Vector_TranscribeSpan(Request request)
{
	//bool isRead = request._type & TRANSCRIBE_COLLECTION_TO_RAW;
	Collection* trgCollection = request._trg;
	Collection* srcCollection = request._src;

	bool barrelTrg = trgCollection->_type->_flags & FLEXIBLE;
	bool barrelSrc = srcCollection->_type->_flags & FLEXIBLE;

	void* writePtr;
	void* readPtr;

	//int currentElement = isRead ? request._srcIx : request._trgIx;
	int direction = request._count < 0 ? -1 : 1;
	int count = request._count * direction;

	int barrelsPerBlock = trgCollection->_type->_barrelsPerBlock;
	int unitsPerBlock = trgCollection->_type->_unitsPerBlock;
	int vectorCount = trgCollection->_count;

	size_t unitSize = trgCollection->_type->_size;
	size_t trgCollectionSize = trgCollection->_capacity * sizeof(Barrel);
	size_t srcCollectionSize = trgCollection->_capacity * sizeof(Barrel);

	size_t trgHead = barrelTrg ? (((BarrelNode*)trgCollection)->_barrelStart * sizeof(Barrel)) + (GlobalBarrelService._heap->_heapStart) : ((Bucket*)trgCollection);
	size_t collectionOffset = trgCollection->_barrelOffset * sizeof(Barrel);

	while (trgCollection->_flags & req_ROLL ||
		trgCollection->_flags & req_FREE) { /* Wait for lock to free */
	}

	InterlockedIncrement(&(trgCollection->_userCount));

	if (srcCollection)
	{
		while (srcCollection->_flags & req_ROLL ||
			srcCollection->_flags & req_FREE) { /* Wait for lock to free */
		}

		InterlockedIncrement(&(srcCollection->_userCount));
	}


	while (count) {

		currentElement = currentElement >= vectorCount ? 0 : currentElement < 0 ? vectorCount - 1 : currentElement;

		*barrelPtr = (Barrel*)(((collectionOffset + (currentElement * unitSize)) % trgCollectionSize) + trgHead);

		if (currentElement % unitsPerBlock ||				// Is not currently started on a 'memory bound' edge
			vectorCount - currentElement < unitsPerBlock ||	// Remaining elements not enough to fill a 'block'
			count < unitsPerBlock)							// Total elements left to write less than one 'block'
		{
			rawTranscribe(readPtr, writePtr, unitSize);

			writePtr = (void*)((size_t)writePtr + unitSize);

			currentElement += direction;
			count--;
		}
		else												// Move entire barrels at a time safely
		{
			barrelTranscribe(readPtr, writePtr, barrelsPerBlock);

			writePtr = (void*)((size_t)writePtr + (sizeof(Barrel) * barrelsPerBlock));

			currentElement += (direction * unitsPerBlock);
			count -= unitsPerBlock;
		}
	}

	InterlockedDecrement(&(trgCollection->_userCount));

	if (srcCollection)
		InterlockedDecrement(&(srcCollection->_userCount));
}