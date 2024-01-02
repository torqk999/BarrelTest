#include <tVector.h>
#include <tHashMap.h>
#include <tBarrel.h>

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

	BarrelNode myIntNode =
	{
		(Vector) {0,intType, heapEnd},
	};
}



void barrelTest()
{
	HeapHandle = HeapCreate(0, 0, PageSize * MaxPageCount);
	if (!HeapHandle)
		return;

	deltaPages(1);

	for (int i = 0; i < 64; i++)
		canvas[i] = 0;

	char input[16];

	BarrelNode boops[BarrelTestCount];

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

		BarrelNode* targetBoop = &boops[index];

	Step_2:

		PREENT("Delta Count: ");

		int delta = strToInt(Geet());
		int newDelta = delta;
		int oldCount = targetBoop->_barrelCount;

		newDelta =
			delta + targetBoop->_barrelCount < 0 ? 0 :
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
			for (int i = 0; i < targetBoop->_barrelCount - oldCount; i++)
			{
				PREENT_ARGS("Input new param (% remaining): ", fmt_i(newDelta - i));
				canvas[(targetBoop->_barrelCount > 0 ? ((i + oldCount + targetBoop->_blockOffset) % targetBoop->_barrelCount) : 0) + targetBoop->_barrelStart] = strToInt(Geet());
			}

	Step_4:

		for (int i = 0; i < BarrelTestCount; i++)
			PREENT_ARGS("[%] count: %  offset: %  start: %  requests: %  flags: %  \n",
				fmt_i(i),
				fmt_i(boops[i]._barrelCount),
				fmt_i(boops[i]._blockOffset),
				fmt_i(boops[i]._barrelStart),
				fmt_i(boops[i]._requests),
				fmt_i(boops[i]._flags));


		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
				PREENT_ARGS("| % ", fmt_i(canvas[(i * 8) + j]));
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
	//hashMapTest();
	//barrelTest();
}
