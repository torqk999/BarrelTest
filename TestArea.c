#include <tVector.h>
#include <tHashMap.h>
#include <tBarrel.h>


#pragma region testRegion
#define BarrelTestCount 10
//void vectorTest()
//{
//	LinkedVector myVector = VEC_TOR(int, 1, 2, 3, 4, 5);
//
//	LinkedVector myNextVector = VEC_TOR(int, 6, 7, 8, 9, 10);
//
//	Vector_Link(&myVector, &myNextVector);
//
//	Vector* vPtr = &(myVector._vector);
//	LinkedVector* lvPtr = &myVector;
//
//	int* ptr = Vector_GetPtr(vPtr, 0);
//	int count = 0;
//
//	int myInt = 42;
//
//	Vector_WriteIndex(vPtr, &myInt, 2);
//
//	while (ptr)
//	{
//		PREENT_ARGS("[%]: %\n", fmt_i(count), fmt_i(*ptr));
//		ptr = LinkedVector_Iterate(&lvPtr, ptr, 1);
//		count++;
//	}
//}

//void hashMapTest()
//{
//	hash_map myHashMap = HASH_MAP(16, .7f);
//
//	struct {
//		char* key;
//		int value;
//	} IntPair[] =
//	{
//		{ "sam" , 5 },
//		{ "bob" , 3 },
//		{ "dan" , 8 }
//	};
//
//	for (int i = 0; i < 3; i++)
//		PREENT_ARGS("Assertion: %\n", fmt_s(hashMap_Assert(&myHashMap, IntPair[i].key, &(IntPair[i].value)) ? "true" : "false"));
//
//	PREENT("\n----------------------------\n\n");
//
//	for (int i = 0; i < 3; i++)
//	{
//		int* intPtr;
//
//		char* result = hashMap_GetOccupantLocation(&myHashMap, IntPair[i].key, &intPtr) ? numConvert((Preem) { 'i', intPtr }) : "FAIL";
//
//		PREENT_ARGS("Value: %\n", fmt_s(result));
//	}
//}

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

	TypeID intType = BARREL_ID(int);

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

	//BarrelNode myIntNode = {
	//
	//		(Vector) {intType,heapEnd, 0}
	//};
}
#pragma endregion

static int TaskFlags = 1;
static ThreadHandle test_threadHandles[MaxThreadCount];
static HeapService test_heapService;
static BarrelService test_barrelService;
static TypeID* test_intTypeID;
static TypeID* test_nodeTypeID;

typedef enum {
	QUIT,
	NEW,
	REMOVE,
	RESIZE,
	WRITE,
	READ,
	VIEW_HEAP
} KeyAction;

bool ESCAPE = false;

typedef struct {
	const char _keyPress;
	const char* _description;
	const void(*_action)();
} ButtonAction;


void barrelTest_ESCAPE() { ESCAPE = true; }

ButtonAction escapeAction = { 'q', "Escape", &barrelTest_ESCAPE };

void fill_screen(char* fill) {
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill == NULL ? ' ' : *fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void clear_screen() { fill_screen(NULL); }

void barrelTest_NEW()
{
	int* targetPtr = NULL;
	const char* input = NULL;

	PREENT("At any time, enter 'q' to leave\nChoose Initial Capacity: ");

	input = Geet();
	if (input[0] == 'q')
		goto End;

	int capacity = strToInt(input);

	if (capacity < 1) {
		PREENT_ARGS("Bad Capacity! Must be greater than 0: %\n", fmt_i(capacity));
		goto End;
	}

	BarrelNode* newNode;
	if (barrel_RequestNode(&test_barrelService, &newNode, (Vector) { test_intTypeID, capacity, capacity}))
		PREENT("Node created!\n");
	else
		PREENT("Node failed to create!\n");
	//test_heapService._heapEnd;

End: {}
}

void barrelTest_REMOVE()
{
	int* targetPtr = NULL;
	const char* input = NULL;
}

void barrelTest_RESIZE()
{

}

void barrelTest_WRITE()
{
	if (test_barrelService.Omegus._vector._count < 1)
	{
		PREENT("No Collections to write to!\n");
		return;
	}

	int* targetPtr = NULL;
	const char* input = NULL;

	PREENT("At any time, enter 'q' to leave\nSet Target: ");

	input = Geet();
	if (input[0] == 'q')
		return;

	int index = strToInt(input);

	if (index < 0 || index >= test_barrelService.Omegus._vector._count) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(test_barrelService.Omegus._vector._count));
		return;
	}

	BarrelNode* targetBoop = barrel_NodeLocation(&test_barrelService, index);

	PREENT("Start Index: ");

	input = Geet();
	if (input[0] == 'q')
		return;

	int startIndex = strToInt(input);

	PREENT("Delta Count: ");

	input = Geet();
	if (input[0] == 'q')
		return;

	int delta = strToInt(input);
	int deltaDir = delta < 0 ? -1 : 1;

	for (int i = 0; i < delta; i += deltaDir)
	{
		PREENT_ARGS("Input new param (% remaining): ", fmt_i((delta - i) * deltaDir));
		targetPtr = barrel_GetElementLocation(&test_barrelService, targetBoop, startIndex + i);
		*targetPtr = strToInt(Geet());
	}

	PREENT("Write Complete!\n");
}

void barrelTest_READ()
{
	if (test_barrelService.Omegus._vector._count < 1)
	{
		PREENT("No Collections to read from!\n");
		return;
	}

	int* targetPtr = NULL;
	const char* input = NULL;

	PREENT("At any time, enter 'q' to leave\nSet Target: ");

	input = Geet();
	if (input[0] == 'q')
		return;

	int index = strToInt(input);

	if (index < 0 || index >= test_barrelService.Omegus._vector._count) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(test_barrelService.Omegus._vector._count));
		return;
	}

	BarrelNode* targetBoop = barrel_NodeLocation(&test_barrelService, index);

	PREENT("Start Index: ");

	input = Geet();
	if (input[0] == 'q')
		return;

	int startIndex = strToInt(input);

	PREENT("Delta Count: ");

	input = Geet();
	if (input[0] == 'q')
		return;

	int delta = strToInt(input);
	int deltaDir = delta < 0 ? -1 : 1;

	for (int i = 0; i < delta; i += deltaDir)
	{
		targetPtr = barrel_GetElementLocation(&test_barrelService, targetBoop, startIndex + i);
		PREENT_ARGS("[%]: %\n", fmt_i(i), fmt_i(*targetPtr));
	}
}

void barrelTest_VIEW_HEAP()
{
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

	PREENT("\n[HEEP MAP]\n");

	for (int i = 0; i < 32; i++) {
		PREENT("|");
		for (int j = 0; j < 8; j++) {
			PREENT_ARGS("| % ", fmt_I(((int*)test_heapService._heapStart)[(i * 8) + j]));
		}

		PREENT("||\n");
	}
}

unsigned int mainMenuActionCount = 8;

ButtonAction mainMenuActions[] = {
	{ 'q', "Quit Program", &barrelTest_ESCAPE },
	{ 'n', "New Collection", &barrelTest_NEW },
	{ 'm', "Remove Collection", &barrelTest_REMOVE },
	{ 's', "Re-size Collection", &barrelTest_RESIZE },
	{ 'r', "Read from Collection span", &barrelTest_READ },
	{ 'w', "Write to Collection span", &barrelTest_WRITE },
	{ 'v', "View entire heap", &barrelTest_VIEW_HEAP },
	{ 'c', "Clear Screen", &clear_screen },
};

bool barrelTest_INIT()
{
	if (!HeapServiceInit(&test_heapService, true))
	{
		PREENT("Init failure!\n");
		return false;
	}

	BarrelServiceInit(&test_barrelService, &test_heapService, test_threadHandles, test_nodeTypeID);

	return true;
}

void barrelTest_MAIN() {

	const char* input = NULL;

	while (!ESCAPE) {

	Step_1:
		PREENT_ARGS("[ Total Current Nodes: % ]\n", fmt_i(test_barrelService.Omegus._vector._count));
		for (int i = 0; i < mainMenuActionCount; i++)
			PREENT_ARGS("[%] - %\n", fmt_c(mainMenuActions[i]._keyPress), fmt_s(mainMenuActions[i]._description));

		input = Geet();

		for (int i = 0; i < mainMenuActionCount; i++)
			if (mainMenuActions[i]._keyPress == input[0])
				mainMenuActions[i]._action();
	}

TestBreak:

	for (int i = 0; i < test_barrelService.Omegus._vector._count; i++)
	{
		BarrelNode* closingNode = barrel_NodeLocation(&test_barrelService, i);
		closingNode->_flags &= ~RUN;
	}

}

void barrelTest()
{
	TypeID intTypeID = TYPE_ID(int);
	TypeID barrelNodeType = TYPE_ID(BarrelNode);

	test_intTypeID = &intTypeID;
	test_nodeTypeID = &barrelNodeType;

	barrelTest_INIT();
	barrelTest_MAIN();
}

int main() {

	//PREENT_ARGS("sizeof Node: %", fmt_l(sizeof(BarrelNode)));
	barrelTest();
}
