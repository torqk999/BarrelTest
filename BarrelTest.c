#include <tBarrel.h>

typedef enum {
	QUIT,
	NEW,
	REMOVE,
	RESIZE,
	WRITE,
	READ,
	VIEW_HEAP
} KeyAction;

typedef enum {
	QuitReminder = 1,
	Indexing = 2,
} MenuOptions;

typedef struct {
	const char _keyPress;
	const char* _description;
	const void(*_action)();
} ButtonAction;

static TypeInfo* test_intTypeID;

bool ESCAPE = false;



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

bool barrel_INPUT(int* outputLoc, const char* inputRequest, int options) {

	if ((options & Indexing) && !Barrel_NodeCount())
	{
		PREENT("No Collections to write to!\n");
		return false;
	}

	const char* input = NULL;

	if (options & QuitReminder)
		PREENT("At any time, enter 'q' to leave\n");
	PREENT_ARGS("%: ", fmt_s(inputRequest));

	input = Geet();
	if (input[0] == 'q')
		return false;

	*outputLoc = strToInt(input);

	if ((options & Indexing) && (*outputLoc < 0 || *outputLoc >= Barrel_NodeCount())) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(Barrel_NodeCount()));
		return false;
	}

	return true;
}

void barrelTest_NEW()
{
	int* targetPtr = NULL;
	const char* input = NULL;
	int capacity;

	if (!barrel_INPUT(&capacity, "Choose Initial Capacity", QuitReminder))
		return;

	if (capacity < 1) {
		PREENT_ARGS("Bad Capacity! Must be greater than 0: %\n", fmt_i(capacity));
		return;
	}

	COLLECTION newNode = Barrel_ctor("int", sizeof(int), NULL, 0, capacity);

	if (newNode) {
		PREENT("Node created!\n");
		newNode->_extensions->_methods(Request(MANAGE, P_(tVARIANT, tRESIZE), P_(tTRG, newNode), P_(tCOUNT, capacity)));
	}

	else
		PREENT("Node failed to create!\n");


	//test_heapService._heapEnd;

}

void barrelTest_REMOVE()
{
	int* targetPtr = NULL;
	const char* input = NULL;
}

void barrelTest_RESIZE()
{
	int index, newSize;

	if (!barrel_INPUT(&index, "Set Target", QuitReminder | Indexing))
		return;

	if (index < 0 || index >= Barrel_NodeCount()) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(Barrel_NodeCount()));
		return;
	}

	BarrelNode* targetBoop = Barrel_GetNode(index);

	if (!barrel_INPUT(&newSize, "New Size", 0))
		return;

	if (newSize < 0) {
		PREENT("New size must be 0 or greater!\n");
		return;
	}

	Collection_Resize(targetBoop, newSize);
}

void barrelTest_WRITE()
{
	int* targetPtr = NULL;
	int index, startIndex, delta;

	if (!barrel_INPUT(&index, "Set Target", QuitReminder | Indexing))
		return;

	if (index < 0 || index >= Barrel_NodeCount()) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(Barrel_NodeCount()));
		return;
	}

	BarrelNode* targetBoop = Barrel_GetNode(index);

	if (!barrel_INPUT(&startIndex, "Start Index", 0))
		return;

	if (!barrel_INPUT(&delta, "Delta Count", 0))
		return;

	int deltaDir = delta < 0 ? -1 : 1;

	for (int i = 0; i != delta; i += deltaDir)
	{
		PREENT_ARGS("Input new param (% remaining): ", fmt_i((delta - i) * deltaDir));
		targetPtr = Barrel_GetUnitPtr(targetBoop, startIndex + i);
		*targetPtr = strToInt(Geet());
	}

	PREENT("Write Complete!\n");
}

void barrelTest_READ()
{
	int* targetPtr = NULL;
	int index, startIndex, delta;

	if (!barrel_INPUT(&index, "Set Target", QuitReminder | Indexing))
		return;

	if (index < 0 || index >= Barrel_NodeCount()) {
		PREENT_ARGS("Bad index! Between 0 and %\n", fmt_i(Barrel_NodeCount()));
		return;
	}

	BarrelNode* targetBoop = Barrel_GetNode(index);

	if (!barrel_INPUT(&startIndex, "Start Index", 0))
		return;

	if (!barrel_INPUT(&delta, "Delta Count", 0))
		return;

	int deltaDir = delta < 0 ? -1 : 1;

	for (int i = 0; i < delta; i += deltaDir) {
		targetPtr = Barrel_GetUnitPtr(targetBoop, startIndex + i);
		PREENT_ARGS("[%]: %\n", fmt_i(i), fmt_i(*targetPtr));
	}
}

void barrelTest_VIEW_HEAP()
{
	PREENT("bCount | vCount | offset | bStart \n");

	for (int i = 0; i < Barrel_NodeCount(); i++)
	{
		BarrelNode* nextNode = Heap_Head();
		PREENT_ARGS("[%]: % | % | % | %\n",
			fmt_I(i),
			fmt_I(nextNode->_barrelCount),
			fmt_I(nextNode->_collection._count),
			fmt_I(nextNode->_barrelOffset),
			fmt_I(nextNode->_barrelStart)
		);
	}

	PREENT("\n[HEEP MAP]\n");

	for (int i = 0; i < 32; i++) {
		PREENT("|");
		for (int j = 0; j < 8; j++) {
			PREENT_ARGS("| % ", fmt_I(((int*)Heap_Head())[(i * 8) + j]));
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
	{ 'r', "Read from Collection to span", &barrelTest_READ },
	{ 'w', "Write to Collection from span", &barrelTest_WRITE },
	{ 'v', "View entire heap", &barrelTest_VIEW_HEAP },
	{ 'c', "Clear Screen", &clear_screen },
};

void barrelTest_MAIN() {

	const char* input = NULL;

	while (!ESCAPE) {

		PREENT_ARGS("[ Total Current Nodes: % ]\n", fmt_i(Barrel_NodeCount()));
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
	PREENT("Collections Testing...\n\n");

	//TypeInfo* intTypeID = TYPE_ID(int, BARREL);

	//test_intTypeID = &intTypeID;

	//long derp = 5;
	//long* ptrDerp = &derp;

	//Request blah = DeConstruct((void*) { 0 });

	if (!Barrel_ServiceInit(Heap_ServiceInit(true)))
	{
		PREENT("Services failed to start!\n");
		return;
	}


	barrelTest_MAIN();
}