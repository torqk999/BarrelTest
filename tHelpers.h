#ifndef	tHelpers
#define	tHelpers
#include <tPreent.h>

//extern CollectionRequest;

REQUEST CreateRequest(RequestType type, Parameter* params, void** buffer);

float Clamp(float value, float min, float max);

int ClampInt(int value, int min, int max);

int pow(int base, unsigned int power);

bool defaultCharCompare(REQUEST request);

bool defaultStringCompare(char* sub, char* str);

int UnsignedIntegralCompare(REQUEST request);

int SignedIntegralCompare(REQUEST request);

int FloatingCompare(REQUEST request);

int findSubStringCustom(const char* src, const char* sub, bool (*compare)(REQUEST request));

int findSubString(const char* src, const char* sub);

unsigned int paramCount(char* rawArray, char* checkArray);

unsigned int paramCount0(const char* params);

void stringTranscribe(char* trg, char* src);

void rawTranscribe(void* trg, void* src, size_t size);

void barrelTranscribe(void* trg, void* src, unsigned int count);

void TranscribeElement(REQUEST request);

void TranscribeSpan(REQUEST request);

#define PARAM_COUNT(...) paramCount( ( char [] ){ __VA_ARGS__ ,  0 },( char [] ) { __VA_ARGS__ , 1 } )

#endif