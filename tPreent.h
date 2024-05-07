

#ifndef	tPreent
#define	tPreent
#include <tModules.h>

#define MAX_CONSOLE_BUFFER 64
#define MAX_NUM_BUFFER 50
#define DBL_PRECISION 15

#define fmt_c(x) { __c, &((char){ x }) }
#define fmt_s(x) { __s,  x }
#define fmt_L(x) { __L, &((long long){ x }) }
#define fmt_I(x) { __I, &((int){ x }) }
#define fmt_b(x) { __b, &((unsigned long long){ x }) }
#define fmt_x(x) { __x, &((unsigned long long){ x }) }
#define fmt_l(x) { __l, &((unsigned long long){ x }) }
#define fmt_O(x) { __O, &((unsigned long long){ x }) }
#define fmt_h(x) { __h, &((unsigned int){ x }) }
#define fmt_i(x) { __i, &((unsigned int){ x }) }
#define fmt_o(x) { __o, &((unsigned int){ x }) }
#define fmt_f(x) { __f, &((float){ x }) }
#define fmt_d(x) { __d, &((double){ x }) }

#define PREENT(string, ...) Preent(string, (Parameter[]) { __VA_ARGS__ })

#define PREENT_SAFE(string, ...) Preent( string , (Parameter[]) { __VA_ARGS__ , nullParam })

static char PREENT_BUFFER[MAX_CONSOLE_BUFFER];

unsigned int strLenSafe(const char* str);

void pootChar(char put, HANDLE output, DWORD written);

void pootStr(const char* str, HANDLE output, DWORD written);

char* integerConvert(unsigned int num, int base);

//char* floatConvert(float num);


int strToInt(const char* input);

char* numConvert(Parameter preem);

const char* Preent(const char* string, Parameter* head);

const char* GeetCount(unsigned int* count);

const char* Geet();

#endif