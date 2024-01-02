#pragma once

#define true 1
#define false 0
#define bool int

#define uint unsigned int
#define ulong unsigned long
#define ullong unsigned long long

#define STR(x) #x
#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define MCLN( x ) MACRO_CONCAT( x, __LINE__ )
#define MCCN( x ) MACRO_CONCAT( x, __COUNTER__ )

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
	for (int i = 0; i < power; i++)
		output *= base;
	return output;
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