#include <tVector.h>
#include <tHashMap.h>

void vectorTest()
{
	LinkedVector myVector = VEC_TOR(int, 1, 2, 3, 4, 5);

	LinkedVector myNextVector = VEC_TOR(int, 6, 7, 8, 9, 10);

	Vector_Link(&myVector, &myNextVector);

	Vector* vPtr = &(myVector._vector);
	LinkedVector* lvPtr = &myVector;

	int* ptr = Vector_GetPtr(vPtr, 0);
	int count = 0;

	int myInt = 42;

	Vector_WriteIndex(vPtr, &myInt, 2);

	while (ptr)
	{
		PREENT_ARGS("[%]: %\n", fmt_i(count), fmt_i(*ptr));
		ptr = LinkedVector_Iterate(&lvPtr, ptr, 1);
		count++;
	}
}

void hashMapTest()
{
	hash_map myHashMap = HASH_MAP(16, .7f);

	struct {
		char* key;
		int value;
	} IntPair[] =
	{
		{ "sam" , 5 },
		{ "bob" , 3 },
		{ "dan" , 8 }
	};

	for (int i = 0; i < 3; i++)
		PREENT_ARGS("Assertion: %\n", fmt_s(hashMap_Assert(&myHashMap, IntPair[i].key, &(IntPair[i].value)) ? "true" : "false"));

	PREENT("\n----------------------------\n\n");

	for (int i = 0; i < 3; i++)
	{
		int* intPtr;

		char* result = hashMap_GetOccupantLocation(&myHashMap, IntPair[i].key, &intPtr) ? numConvert((Preem) { 'i', intPtr }) : "FAIL";

		PREENT_ARGS("Value: %\n", fmt_s(result));
	}
}



DWORD WINAPI MyThreadJob(void* dataHead)
{
	if (!dataHead)
		return 0;

	int* target = dataHead;
	int* flags = &(target[1]);


	while (!(*flags))
	{
		(*target)++;
		Sleep(1000);
	}

	return 0;
}

void threadTest()
{
	DWORD threadID;

	int count[2];

	count[0] = 0;
	count[1] = 0;

	HANDLE myThread = CreateThread(
		NULL,
		0,
		&MyThreadJob,
		count,
		0,
		&threadID
	);

	if (!myThread)
		return;

	char buffer[64];

	Geet(buffer);

	count[1] = 1;

	DWORD result = WaitForSingleObject(myThread, INFINITE);

	PREENT_ARGS("Waited seconds: %", fmt_i(*count));
}

static uint PageCount = 0;
const uint MaxPageCount = 64;
static HANDLE HeapHandle;
static size_t PageSize = 4096;

void deltaPages(int delta)
{
	delta = delta < 0 && (delta * -1) > PageCount ? PageCount * -1 : delta;

	if (delta > 0)
		HeapAlloc(HeapHandle, 0, delta * PageSize);

	PageCount += delta;

	if (delta < 0)
		HeapFree(HeapHandle, 0, (void*)((size_t)HeapHandle + (PageCount * PageCount)));

}

void heapTest()
{
	HeapHandle = HeapCreate(0, 0, PageSize * MaxPageCount);
	if (!HeapHandle)
		return;

	deltaPages(1);

	TypeID intType = TYPE_ID(int);

	void* heapEnd = HeapHandle;

	int barrelsPerBlock,
		unitsPerBlock;

	if (!(Barrel_Size_Unit % intType._size))
	{
		unitsPerBlock = Barrel_Size_Unit / intType._size;
		barrelsPerBlock = 1;
	}

	else if (!(intType._size % Barrel_Size_Unit))
	{
		barrelsPerBlock = intType._size / Barrel_Size_Unit;
		unitsPerBlock = 1;
	}

	else
	{

	}

	barrel_node myIntNode =
	{
		(Vector) {0,intType, heapEnd},
	};
}

enum testFlags
{
	RUN = 1,
	DONE = 2,
	req_ROLL = 4,
	req_FREE = 8,
	wait_ROLL = 16,
	wait_FREE = 32
};

enum testConsts
{
	BarrelTestCount = 10,
	DeltaBufferCount = 10
};

typedef struct boop
{
	// Barrel Section
	int _start;
	int _count;
	int _offset;
	int _requests;

	// Thread Section
	DWORD _ID;
	HANDLE _handle;
	struct boop* _bucket;
	int* _data;
	int _index;
	int _flags;
} Boop;

void barrelTestRoll(Boop* boop, int delta, int dir)
{
	// dir: -1 = left 1 = right

	if (dir == 0)
		return;

	int absDelta = delta < 0 ? delta * -1 : delta;

	if (boop->_count > 0)
	{
		int last = boop->_start + (boop->_count - 1);

		// full roll

		if (delta == 0)
		{

			if (dir > 0)
			{
				boop->_data[last + 1] = boop->_data[boop->_start];
				boop->_offset--;
				boop->_start++;
			}

			else
			{
				boop->_data[boop->_start - 1] = boop->_data[last];
				boop->_offset++;
				boop->_start--;
			}

			//boop->_start += dir > 0 ? 1 : -1;
		}

		// with no offset, the end is guaranteed, no action needed

		//   _ ||||||||||||| _

		else
		{
			// tail end rolling

			if ((delta < 0 && dir < 0) ||
				(delta > 0 && dir > 0))
			{


				for (int j = 0; j < absDelta; j++)
				{

					if (dir > 0) // roll forward    ||||||| -> |||||||
					{
						if (boop->_offset > 0)
							for (int i = last; i >= boop->_start + boop->_offset; i--)
								boop->_data[i + 1] = boop->_data[i];

						//boop->_start++;
					}


					else // roll backward           ||||||| <- |||||||
					{
						if (boop->_offset > 0)
							for (int i = boop->_start + boop->_offset; i < boop->_start + (boop->_count - 1); i++)
								boop->_data[i] = boop->_data[i + 1];

						//boop->_start--;
					}
				}

			}

			// head end rolling

			else
			{
				for (int j = 0; j < absDelta; j++)
				{

					if (dir > 0) // roll forward    ||||||| -> |||||||
					{
						if (boop->_offset > 0)
							for (int i = boop->_start + (boop->_offset - 2); i >= boop->_start; i--)
								boop->_data[i + 1] = boop->_data[i];

						boop->_offset--;
						boop->_start++;
					}

					else // roll backward           ||||||| <- |||||||
					{
						if (boop->_offset > 0)
							for (int i = boop->_start; i < boop->_start + (boop->_offset - 1); i++)
								boop->_data[i] = boop->_data[i + 1];

						boop->_offset++;
						boop->_start--;
					}
				}
			}
		}
	}

	else if (delta == 0 ||
		(delta < 0 && dir > 0) ||
		(delta > 0 && dir < 0))
		boop->_start += dir > 0 ? 1 : -1;

	boop->_count += delta;

	boop->_requests -= delta;

	boop->_count = boop->_count < 0 ? 0 : boop->_count;

	boop->_offset =
		boop->_offset < 0 ?
		boop->_count > boop->_offset * -1 ? (boop->_offset + boop->_count) % boop->_count : 0 :
		boop->_offset > boop->_count ?
		boop->_count > 0 ? boop->_offset % boop->_count : 0 :
		boop->_offset;
}

DWORD WINAPI barrelTestWork(void* target)
{
	if (!target)
		return 1;

	Boop* boop = target;

	while (boop->_flags & RUN)
	{
		int delta = boop->_requests < 0 ? -1 : boop->_requests > 0 ? 1 : 0;

		if (boop->_flags & req_ROLL)
		{
			// take this opportunity to shrink the barrel in place by 1 and allow the immediate roll forward
			if (boop->_requests < 0)
			{
				barrelTestRoll(boop, -1, 1);
				boop->_flags &= ~req_ROLL;
			}

			// check for last index for automatic rolling rights
			else if (boop->_index + 1 == BarrelTestCount)
			{
				barrelTestRoll(boop, boop->_requests, 1);
				boop->_flags &= ~req_ROLL;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(boop->_flags & wait_ROLL))
			{
				boop->_bucket[boop->_index + 1]._flags |= req_ROLL;
				boop->_flags |= wait_ROLL;
			}

			else if (boop->_flags & wait_ROLL &&
				!(boop->_bucket[boop->_index + 1]._flags & req_ROLL))
			{
				barrelTestRoll(boop, delta, 1);
				boop->_flags &= ~wait_ROLL;
				boop->_flags &= ~req_ROLL;
			}
		}

		else if (boop->_flags & req_FREE)
		{
			// take this opportunity to grow the barrel in place by 1 and allow the immediate roll backward
			if (boop->_requests > 0)
			{
				barrelTestRoll(boop, 1, -1);
				boop->_flags &= ~req_FREE;
			}

			// check for last index for automatic rolling rights
			else if (boop->_index + 1 == BarrelTestCount)
			{
				barrelTestRoll(boop, 0, 1);
				boop->_flags &= ~req_FREE;
			}

			// inform the prev barrel that there is a requested slot to roll to
			else if (!(boop->_flags & wait_FREE))
			{
				boop->_bucket[boop->_index + 1]._flags |= req_FREE;
				boop->_flags |= wait_FREE;
			}

			else if ((boop->_flags & wait_FREE) &&
				!(boop->_bucket[boop->_index + 1]._flags & req_FREE))
			{
				barrelTestRoll(boop, delta, -1);
				boop->_flags &= ~wait_FREE;
				boop->_flags &= ~req_FREE;
			}
		}

		else if (boop->_requests != 0)
		{
			boop->_flags |= boop->_requests < 0 ? req_FREE : req_ROLL;
		}
	}

	boop->_flags |= DONE;

	return 0;
}

void boop_ctor(Boop* bucket, int index, int* dataHead)
{
	bucket[index]._bucket = bucket;
	bucket[index]._start = 0;
	bucket[index]._count = 0;
	bucket[index]._offset = 0;
	bucket[index]._requests = 0;

	bucket[index]._index = index;
	bucket[index]._data = dataHead;
	bucket[index]._handle = CreateThread(NULL, 0, barrelTestWork, &bucket[index], 0, &(bucket[index]._ID));
	bucket[index]._flags = RUN;

	if (!(bucket[index]._handle))
		PREENT("thread failed to execute!\n");
}

//int boop_delta(Boop* boop, int delta, int* bucket)
//{
//	boop->_requests = delta;
//
//	//SYSTEMTIME start;
//	//GetSystemTime(&start);
//
//	while (true)
//	{
//		//SYSTEMTIME current;
//		//GetSystemTime(&current);
//		//if (current.wSecond - start.wSecond > 5)
//		//	return 0;
//
//		if (boop->_requests == 0)
//			break;
//	}
//
//	if (delta > 0)
//		for (int i = 0; i < delta; i++)
//		{
//			PREENT_ARGS("%", fmt_i(bucket[i]));
//		}
//}

void barrelTest()
{
	int canvas[256];

	for (int i = 0; i < 256; i++)
		canvas[i] = 0;

	char input[16];

	Boop boops[BarrelTestCount];

	for (int i = 0; i < BarrelTestCount; i++)
		boop_ctor(boops, i, canvas);

	int intBuffer[DeltaBufferCount];

	while (true)
	{
	Step_1:

		PREENT("Set Target: ");
		int index = strToInt(Geet());

		if (index < 0 || index >= BarrelTestCount)
		{
			PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(BarrelTestCount));
			goto Step_1;
		}

		Boop* targetBoop = &boops[index];

	Step_2:

		PREENT("Delta Count: ");

		int delta = strToInt(Geet());
		int newDelta = delta;
		int oldCount = targetBoop->_count;

		newDelta =
			delta + targetBoop->_count < 0 ? 0 :
			delta > DeltaBufferCount ? DeltaBufferCount :
			delta;

		if (newDelta != delta)
			PREENT_ARGS("Delta Capped: %\n", fmt_i(newDelta));

	Step_3:

		targetBoop->_requests += newDelta;

		while (targetBoop->_requests)
		{
			PREENT_ARGS("waiting on requests: %\n", fmt_I(targetBoop->_requests));
			Sleep(250);
		}

		PREENT("requests complete!\n");

		if (newDelta > 0)
			for (int i = 0; i < targetBoop->_count - oldCount; i++)
			{
				PREENT_ARGS("Input new param (% remaining): ", fmt_i(newDelta - i));
				canvas[(targetBoop->_count > 0 ? ((i + oldCount + targetBoop->_offset) % targetBoop->_count) : 0) + targetBoop->_start] = strToInt(Geet());
			}

	Step_4:

		for (int i = 0; i < BarrelTestCount; i++)
			PREENT_ARGS("[%] count: %  offset: %  start: %  requests: %  flags: %  \n",
				fmt_i(i),
				fmt_i(boops[i]._count),
				fmt_i(boops[i]._offset),
				fmt_i(boops[i]._start),
				fmt_i(boops[i]._requests),
				fmt_i(boops[i]._flags));


		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
				PREENT_ARGS("| % ", fmt_i(canvas[(i * 16) + j]));
			PREENT("|\n");
		}
	}


	for (int i = 0; i < BarrelTestCount; i++)
		boops[i]._flags &= ~RUN;

	for (int i = 0; i < BarrelTestCount; i++)
		if (!(boops[i]._flags & DONE))
			i = 0;

	//WaitForMultipleObjects(BarrelTestCount, threadHandles, true, INFINITE);

	for (int i = 0; i < BarrelTestCount; i++)
		if (boops[i]._handle)
			CloseHandle(boops[i]._handle);
}

int main()
{

	barrelTest();
}
