#include <tHelpers.h>

REQUEST CreateRequest(RequestType type, Parameter* params, void** buffer)
{
	REQUEST output = { type, buffer };

	for (int i = 0; i < MaxParamCount; i++)
		buffer[i] = NONE;

	uint seek = 0;
	while (params[seek]._type > -1 && params[seek]._type < MaxParamCount) {
		buffer[params[seek]._type] = params[seek]._ptr;
		seek++;
	}
	buffer[seek] = NULL;
	return output;
}

float Clamp(float value, float min, float max)
{
	value = value < min ? min : value > max ? max : value;
}

int ClampInt(int value, int min, int max)
{
	value = value < min ? min : value > max ? max : value;
}

int pow(int base, unsigned int power)
{
	int output = 1;
	for (unsigned int i = 0; i < power; i++)
		output *= base;
	return output;
}

bool defaultCharCompare(REQUEST request)
{
	//return ((char*)request._src)[request._srcIx] == ((char*)request._trg)[request._trgIx];
}

bool defaultSubStringCompare(char* sub, char* str)
{
	int seek = 0;
	while (sub[seek] != '\0') {
		if (sub[seek] != str[seek])
			return false;
		seek++;
	}
	return true;
}

bool defaultStringCompare(char* sub, char* str)
{
	int seek = 0;
	while (sub[seek] != '\0') {
		if (sub[seek] != str[seek])
			return false;
		seek++;
	}
	return str[seek] == '\0';
}

int UnsignedIntegralCompare(REQUEST request)
{
	//ullong* left = ((ullong*)request._src);
	//ullong* right = ((ullong*)request._trg);
	//
	//return left[request._srcIx] > right[request._trgIx] ? 1 : left[request._srcIx] < right[request._trgIx] ? -1 : 0;
}

int SignedIntegralCompare(REQUEST request)
{
	//llong* left = ((llong*)request._src);
	//llong* right = ((llong*)request._trg);
	//
	//return left[request._srcIx] > right[request._trgIx] ? 1 : left[request._srcIx] < right[request._trgIx] ? -1 : 0;
}

int FloatingCompare(REQUEST request)
{
	//long double* left = ((long double*)request._src);
	//long double* right = ((long double*)request._trg);
	//
	//return left[request._srcIx] > right[request._trgIx] ? 1 : left[request._srcIx] < right[request._trgIx] ? -1 : 0;
}

int findSubStringCustom(const char* sub, const char* src, bool (*compare)(REQUEST request))
{
	//int seek = 0;
	//
	//while (src[seek] != '\0') {
	//
	//	if (compare(Request_Compare())
	//		return seek;
	//	seek++;
	//}
	return -1;
}


int findSubString(const char* sub, const char* src) {

	int seek = 0;

	while (src[seek] != '\0') {

		if (defaultSubStringCompare(sub, src))
			return seek;
			seek++;
	}
	return -1;
}

unsigned int paramCount(char* rawArray, char* checkArray)
{
	unsigned int count = 0;

	while (true)
	{
		if (rawArray[count] != checkArray[count])
			break;
		count++;
	}

	return count;
}

unsigned int paramCount0(const char* params)
{
	if (params[0] == '\0')
		return 0;
	int paramCount = 1;
	int scopeLevel = 0;
	for (char* i = (char*)params; *i != '\0'; i++)
		switch (*i)
		{
		case '{':
		case '(':
		case '[':
		case '<':
			scopeLevel++;
			break;

		case '}':
		case ')':
		case ']':
		case '>':
			scopeLevel--;
			break;

		case ',':
			paramCount += scopeLevel == 0;
			break;
		}
	return paramCount;
}

void stringTranscribe(char* trg, char* src) {
	int seek = 0;
	while (src[seek] != '\0') {
		trg[seek] = src[seek];
		seek++;
	}
	trg[seek] = '\0';
}

void rawTranscribe(void* trg, void* src, size_t size)
{
	for (size_t i = 0; i < size; i++)
		((char*)trg)[i] = src ? ((char*)src)[i] : NULL;
}

void barrelTranscribe(void* trg, void* src, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
		((Barrel*)trg)[i] = ((Barrel*)src)[i];
}

void TranscribeElement(REQUEST request)
{
	//rawTranscribe(
	//	request._src ?
	//		((size_t)request._src + (request._srcIx * request._size)) :
	//		NULL,
	//	((size_t)request._trg + (request._trgIx * request._size)),
	//	request._size);
}

void TranscribeSpan(REQUEST request)
{
	//int dir = request._size < 0 ? -1 : 1;
	//unsigned long long count = request._size * dir;
	//
	//for (unsigned long long i = 0; i < count; i++)
	//{
	//	TranscribeElement(request);
	//	request._srcIx += dir;
	//	request._trgIx += dir;
	//}

}

