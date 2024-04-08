#include <tBarrel.h>
#include <tHashMap.h>


static TypeInfo* test_intTypeID;

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
	if (barrel_RequestNode(&newNode, (Bucket) { test_intTypeID, capacity, capacity}))
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
	if (barrel_NodeCount() < 1)
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

	if (index < 0 || index >= barrel_NodeCount()) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(barrel_NodeCount()));
		return;
	}

	BarrelNode* targetBoop = barrel_NodeLocation(index);

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
		targetPtr = barrel_GetPtr(targetBoop, startIndex + i);
		*targetPtr = strToInt(Geet());
	}

	PREENT("Write Complete!\n");
}

void barrelTest_READ()
{
	if (barrel_NodeCount() < 1)
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

	if (index < 0 || index >= barrel_NodeCount()) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(barrel_NodeCount()));
		return;
	}

	BarrelNode* targetBoop = barrel_NodeLocation(index);

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
		targetPtr = barrel_GetPtr(targetBoop, startIndex + i);
		PREENT_ARGS("[%]: %\n", fmt_i(i), fmt_i(*targetPtr));
	}
}

void barrelTest_VIEW_HEAP()
{
	PREENT("bCount | vCount | offset | bStart | bRequests | flags\n");

	for (int i = 0; i < barrel_NodeCount(); i++)
	{
		BarrelNode nextNode = *barrel_NodeLocation(i);
		PREENT_ARGS("[%]: % | % | % | % | % | % \n",
			fmt_I(i),
			fmt_I(nextNode._managed._collection._capacity),
			fmt_I(nextNode._managed._collection._count),
			fmt_I(nextNode._barrelOffset),
			fmt_I(nextNode._barrelStart),
			fmt_I(nextNode._requests),
			fmt_I(nextNode._flags));
	}

	PREENT("\n[HEEP MAP]\n");

	//for (int i = 0; i < 32; i++) {
	//	PREENT("|");
	//	for (int j = 0; j < 8; j++) {
	//		PREENT_ARGS("| % ", fmt_I(((int*)test_heapService->_heapStart)[(i * 8) + j]));
	//	}
	//
	//	PREENT("||\n");
	//}
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

void barrelTest_MAIN() {

	const char* input = NULL;

	while (!ESCAPE) {

		PREENT_ARGS("[ Total Current Nodes: % ]\n", fmt_i(barrel_NodeCount()));
		for (int i = 0; i < mainMenuActionCount; i++)
			PREENT_ARGS("[%] - %\n", fmt_c(mainMenuActions[i]._keyPress), fmt_s(mainMenuActions[i]._description));

		input = Geet();

		for (int i = 0; i < mainMenuActionCount; i++)
			if (mainMenuActions[i]._keyPress == input[0])
				mainMenuActions[i]._action();
	}
}

void barrelTest()
{
	TypeInfo* intTypeID = TYPE_ID(int, BARREL);

	//test_intTypeID = &intTypeID;

	long derp = 5;
	long* ptrDerp = &derp;

	Request blah = DeConstruct((void*) { 0 });

	if (!BarrelServiceInit(HeapServiceInit(true)))
	{
		PREENT("Services failed to start!\n");
		return;
	}


	barrelTest_MAIN();
}

//#define Derp(x) _Generic


int main() {

	TypeInfo* myIntType = TYPE_ID(int, BUCKET);

	//PREENT_ARGS("sizeof Node: %\n %", fmt_l(sizeof(BarrelNode)));
	barrelTest();
}




/*

Instruction - WORD SIZE
Address
Value - WORD SIZE

PROCESS 
///////////////
READ-ONLY 0xFF...

STACK
|
V

/\/\/\/\/\/\ <- OS dependant

^
|
HEAP 0x00...
////////////////

*/