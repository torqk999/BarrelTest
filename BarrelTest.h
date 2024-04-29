#pragma once
#include <tBarrel.h>



void barrelTest_ESCAPE();

void fill_screen(char* fill);

void clear_screen();

bool barrel_INPUT(int* outputLoc, const char* inputRequest, int options);

void barrelTest_NEW();

void barrelTest_REMOVE();

void barrelTest_RESIZE();

void barrelTest_WRITE();

void barrelTest_READ();

void barrelTest_VIEW_HEAP();

void barrelTest_MAIN();

void barrelTest();