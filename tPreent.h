

#ifndef	tPreent
#define	tPreent
#include <tModules.h>

#define MAX_CONSOLE_BUFFER 64
#define MAX_NUM_BUFFER 50
#define DBL_PRECISION 15

#define fmt_c(x) { 'c', &((char){ x }) }
#define fmt_s(x) { 's',  x }
#define fmt_L(x) { 'L', &((long long){ x }) }
#define fmt_I(x) { 'I', &((int){ x }) }
#define fmt_x(x) { 'x', &((unsigned long long){ x }) }
#define fmt_l(x) { 'l', &((unsigned long long){ x }) }
#define fmt_O(x) { 'O', &((unsigned long long){ x }) }
#define fmt_h(x) { 'h', &((unsigned int){ x }) }
#define fmt_i(x) { 'i', &((unsigned int){ x }) }
#define fmt_o(x) { 'o', &((unsigned int){ x }) }
#define fmt_f(x) { 'f', &((float){ x }) }
#define fmt_d(x) { 'd', &((double){ x }) }

#define PREENT(string, ...) Preent(string, NULL)

#define PREENT_ARGS(string, ...) Preent( string , (Preem[]) { __VA_ARGS__ , { 'n', NULL } })

static char PREENT_BUFFER[MAX_CONSOLE_BUFFER];

unsigned int strLenSafe(const char* str);

void pootChar(char put, HANDLE output, DWORD written);

void pootStr(const char* str, HANDLE output, DWORD written);

char* integerConvert(unsigned int num, int base);

//char* floatConvert(float num);


int strToInt(const char* input);

char* numConvert(Preem preem);

const char* Preent(const char* string, Preem* head);

const char* GeetCount(unsigned int* count);

const char* Geet();

#endif