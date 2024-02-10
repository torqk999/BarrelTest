#include <tVector.h>
#include <tHashMap.h>
#include <tBarrel.h>

#pragma region testRegion
#define BarrelTestCount 10
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

	//DWORD result = WaitForSingleObject(myThread, INFINITE);

	//PREENT_ARGS("Waited seconds: %", fmt_i(*count));
}
void heapTest(HeapService* heapService)
{
	heapService->_heapStart = HeapCreate(0, 0, PageSize * MaxPageCount);
	if (!heapService->_heapStart)
		return;

	heapDeltaPages(heapService, 1);

	TypeID intType = TYPE_ID(int);

	void* heapEnd = heapService->_heapStart;

	int barrelsPerBlock,
		unitsPerBlock;

	if (!(sizeof(Barrel) % intType._size))
	{
		unitsPerBlock = sizeof(Barrel) / intType._size;
		barrelsPerBlock = 1;
	}

	else if (!(intType._size % sizeof(Barrel)))
	{
		barrelsPerBlock = intType._size / sizeof(Barrel);
		unitsPerBlock = 1;
	}

	else
	{

	}

	BarrelNode myIntNode = {

			(Vector) {intType,heapEnd, 0}
	};
}
#pragma endregion

static int TaskFlags = 1;
static ThreadHandle test_threadHandles[MaxThreadCount];
static HeapService test_heapService;
static BarrelService test_barrelService;

void barrelTest_REMOVE()
{
	int* targetPtr = NULL;
	const char* input = NULL;
}

void barrelTest_NEW()
{
	int* targetPtr = NULL;
	const char* input = NULL;

	PREENT("Choose Initial Capacity: ");

	input = Geet();
	if (input[0] == 'q')
		goto End;

	int capacity = strToInt(input);

	if (capacity < 1) {
		PREENT_ARGS("Bad Capacity! Must be greater than 0: %\n", fmt_i(capacity));
		goto End;
	}



	//test_heapService._heapEnd;

End: {}
}

void barrelTest_EDIT()
{
	int* targetPtr = NULL;
	const char* input = NULL;

	PREENT("Set Target: ");

	input = Geet();
	if (input[0] == 'q')
		goto End;

	int index = strToInt(input);

	if (index < 1 || index >= test_barrelService.Omegus._vector._count) {
		PREENT_ARGS("Bad index! Between 1 and %\n", fmt_i(test_barrelService.Omegus._vector._count));
		goto End;
	}

	BarrelNode* targetBoop = barrel_NodeLocation(&test_barrelService, index);

	PREENT("Delta Count: ");

	input = Geet();
	if (input[0] == 'q')
		goto End;

	int delta = strToInt(input);
	int newDelta = delta;

	newDelta =
		delta + targetBoop->_barrelCount < 0 ? 0 :
		delta;

	if (newDelta != delta)
		PREENT_ARGS("Delta Capped: %\n", fmt_i(newDelta));

	if (!barrel_VectorResizeRequest(targetBoop, newDelta, NULL))
		goto PREENT;

	while (targetBoop->_requests)
	{
		PREENT_ARGS("waiting on requests: %\n", fmt_I(targetBoop->_requests));
		Sleep(250);
	}

	PREENT("requests complete!\n");

PREENT:
	{
		int oldCount = targetBoop->_vector._count;
		targetBoop->_vector._count += newDelta;

		if (newDelta > 0)
			for (int i = oldCount; i < targetBoop->_vector._count; i++)
			{
				PREENT_ARGS("Input new param (% remaining): ", fmt_i(newDelta - (targetBoop->_vector._count - i)));
				if (barrel_GetElementLocation(&test_barrelService, targetBoop, i, &targetPtr))
					*targetPtr = strToInt(Geet());
			}
	}

	PREENT("bCount | vCount | offset | bStart | bRequests | flags\n");

	for (int i = 0; i < test_barrelService.Omegus._vector._count; i++)
	{
		BarrelNode nextNode = *barrel_NodeLocation(&test_barrelService, i);
		PREENT_ARGS("[%]: % | % | % | % | % | % \n",
			fmt_I(i),
			fmt_I(nextNode._barrelCount),
			fmt_I(nextNode._vector._count),
			fmt_I(nextNode._barrelOffset),
			fmt_I(nextNode._barrelStart),
			fmt_I(nextNode._requests),
			fmt_I(nextNode._flags));
	}

	for (int i = 0; i < 32; i++) {
		PREENT("|");
		for (int j = 0; j < 8; j++) {
			PREENT_ARGS("| % ", fmt_I(((int*)test_heapService._heapStart)[(i * 8) + j]));
		}

		PREENT("||\n");
	}

End: {}
}

void barrelTest() {

	// Thread and service initializers

	if (!HeapServiceInit(&test_heapService, true))
	{
		PREENT("Init failure!\n");
		return;
	}

	BarrelServiceInit(&test_barrelService, &test_heapService, test_threadHandles);

	const char* input = NULL;

	while (true) {

	Step_1:
		PREENT("Choose Option (q == quit, w == new, e == edit, r == remove): ");

		input = Geet();
		switch (input[0])
		{
		case 'q':
			goto TestBreak;

		case 'w':
			barrelTest_NEW();
			break;

		case 'e':
			barrelTest_EDIT();
			break;

		case 'r':
			barrelTest_REMOVE();
			break;

		default:
			PREENT("Unknown command!\n");
			break;
		}
	}

TestBreak:

	for (int i = 0; i < test_barrelService.Omegus._vector._count; i++)
	{
		BarrelNode* closingNode = barrel_NodeLocation(&test_barrelService, i);
		closingNode->_flags &= ~RUN;
	}

}

int main() {

	//PREENT_ARGS("sizeof Node: %", fmt_l(sizeof(BarrelNode)));
	barrelTest();
}
