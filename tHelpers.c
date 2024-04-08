#include <tHelpers.h>

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

bool defaultCharCompare(Request request)
{
	return ((char*)request._src)[request._srcIx] == ((char*)request._trg)[request._trgIx];
}

bool defaultStringCompare(Request request)
{
	while (((char*)request._src)[request._srcIx] != '\0')
	{
		if (((char*)request._src)[request._srcIx] != ((char*)request._trg)[request._trgIx])
			return false;

		request._srcIx++;
		request._trgIx++;
	}
	return ((char*)request._trg)[request._trgIx] == '\0';
}

int UnsignedIntegralCompare(Request request)
{
	ullong* left = ((ullong*)request._src);
	ullong* right = ((ullong*)request._trg);

	return left[request._srcIx] > right[request._trgIx] ? 1 : left[request._srcIx] < right[request._trgIx] ? -1 : 0;
}

int SignedIntegralCompare(Request request)
{
	llong* left = ((llong*)request._src);
	llong* right = ((llong*)request._trg);

	return left[request._srcIx] > right[request._trgIx] ? 1 : left[request._srcIx] < right[request._trgIx] ? -1 : 0;
}

int FloatingCompare(Request request)
{
	long double* left = ((long double*)request._src);
	long double* right = ((long double*)request._trg);

	return left[request._srcIx] > right[request._trgIx] ? 1 : left[request._srcIx] < right[request._trgIx] ? -1 : 0;
}

int findSubStringCustom(const char* src, const char* sub, bool (*compare)(Request request))
{
	int currentIndex = 0;
	const char* currentCheck = sub;
	while (*src != '\0') {
		if (currentCheck == '\0')
			return currentIndex;

		currentCheck = compare((Request) { COMPARE_EQUIVALENCE, src, currentCheck, 0, 0, 1 }) ? currentCheck + 1 : sub;
		src++;
	}
	return -1;
}


int findSubString(const char* src, const char* sub) { return findSubStringCustom(src, sub, &defaultCharCompare); }

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

void TranscribeElement(Request request)
{
	rawTranscribe(
		request._src ?
			((size_t)request._src + (request._srcIx * request._size)) :
			NULL,
		((size_t)request._trg + (request._trgIx * request._size)),
		request._size);
}

void TranscribeSpan(Request request)
{
	int dir = request._size < 0 ? -1 : 1;
	unsigned long long count = request._size * dir;

	for (unsigned long long i = 0; i < count; i++)
	{
		TranscribeElement(request);
		request._srcIx += dir;
		request._trgIx += dir;
	}

}

