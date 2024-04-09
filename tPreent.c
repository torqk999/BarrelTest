#include <tPreent.h>

unsigned int strLenSafe(const char* str)
{
	unsigned int count = 1;
	while (str[count - 1] != '\0')
	{
		if (count > MAX_CONSOLE_BUFFER)
			return 0;
		count++;
	}

	return count;
}

void pootChar(char put, HANDLE output, DWORD written)
{
	//HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD written = 0;

	WriteConsoleA(output, &put, 1, &written, NULL);
}

void pootStr(const char* str, HANDLE output, DWORD written)
{
	//HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD written = 0;

	//while (*str != '\0')
	//{
	unsigned int length = strLenSafe(str);

	if (!length)
		return;

	WriteConsoleA(output, str, length, &written, NULL);
	//str++;
//}
}

char* integerConvert(unsigned int num, int base)
{
	static char Representation[] = "0123456789ABCDEF";
	static char iBuffer[MAX_NUM_BUFFER];


	char* ptr = &iBuffer[MAX_NUM_BUFFER - 1];
	*ptr = '\0';

	do
	{
		ptr -= 1;
		*ptr = Representation[num % base];
		num /= base;
	} while (num != 0);

	return(ptr);
}
//
//char* floatConvert(float num)
//{
//
//
//}

int strToInt(const char* input)
{
	int numStart = 0;
	int isNegative = 0;
	int output = 0;

	char start = '0';
	char end = '9';

	while (*input != '\0')
	{
		if (!numStart && *input == '-')
			isNegative = 1;


		if (*input >= '0' && *input <= '9')
		{
			numStart = 1;

			output *= 10;
			output += *input - 48;
		}
		else if (numStart)
			break;

		input++;
	}

	output *= isNegative ? -1 : 1;

	return output;
}

char* numConvert(Parameter preem)
{
	static char Representation[] = "0123456789ABCDEF";
	static char Buffer[MAX_NUM_BUFFER];

	char* ptr = &Buffer[MAX_NUM_BUFFER - 1];
	*ptr = '\0';

	char next;
	int isPositive = 1;
	int isInt = 0;

	int intCheck;
	long long longCheck;

	double fVal = 0;
	unsigned long long iVal = 0;
	unsigned long long fRem = 0;
	unsigned int base = 0;

	switch (preem._type)
	{
	case 'I':
		intCheck = *((int*)(preem._ptr));
		isPositive = intCheck >= 0;
		iVal = isPositive ? intCheck : -intCheck;
		base = 10;
		goto IsInt;
	case 'L':
		longCheck = *((long long*)(preem._ptr));
		isPositive = longCheck >= 0;
		iVal = isPositive ? longCheck : -longCheck;
		base = 10;
		goto IsInt;
	case 'x':
		base += 6;
	case 'l':
		base += 2;
	case 'O':
		base += 8;
		iVal = *((unsigned long long*)(preem._ptr));
		goto IsInt;
	case 'h':
		base += 6;
	case 'i':
		base += 2;
	case 'o':
		base += 8;
		iVal = *((unsigned int*)(preem._ptr));
	IsInt:
		isInt = 1;
		break;

	case 'f':
		fVal = *((float*)(preem._ptr));
		goto IsFloat;
	case 'd':
		fVal = *((double*)(preem._ptr));
	IsFloat:
		isPositive = fVal >= 0;
		fVal *= isPositive ? 1 : -1;
		iVal = fVal;
		fVal -= iVal;
		base = 10;
		isInt = 0;
	}

	if (!isInt)
	{
		if (fVal > 0)
		{
			do
			{
				fVal *= 10;
				fRem = fVal;
			} while (fVal - (double)fRem > 0);

			do
			{
				ptr -= 1;
				*ptr = Representation[fRem % base];
				fRem /= base;

			} while (fRem != 0);

			ptr -= 1;
			*ptr = '.';
		}
	}

	do
	{
		ptr -= 1;
		*ptr = Representation[iVal % base];
		iVal /= base;

	} while (iVal != 0);

	if (!isPositive)
	{
		ptr -= 1;
		*ptr = '-';
	}

	return(ptr);
}

const char* Preent(const char* string, Parameter* head)
{
	unsigned int startIndex = 0;
	char* workingIndex = PREENT_BUFFER;

	void* ptr = head;

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written = 0;

	for (int i = 0; i < MAX_CONSOLE_BUFFER; i++)
	{
		PREENT_BUFFER[i] = string[i];

		if (string[i] == '\0')
		{
			pootStr(workingIndex, output, written);
			return NULL;
		}

		if (string[i] == '%')
		{
			PREENT_BUFFER[i] = '\0';
			pootStr(workingIndex, output, written);
			workingIndex = &PREENT_BUFFER[i + 1];

			switch (head[0]._type)
			{
			case __c:
				pootChar(*((char*)(head[0]._ptr)), output, written);
				head++;
				break;

			case __s:
				pootStr((char*)(head[0]._ptr), output, written);
				head++;
				break;

			case __I:
			case __L:
			case __x:
			case __l:
			case __O:
			case __h:
			case __i:
			case __o:
			case __f:
			case __d:
				pootStr(numConvert(head[0]), output, written);
				head++;
				break;

			case NONE:
				pootStr("\n[NULL PARAM ERROR]\n", output, written);
				break;

			default:
				break;
			}
		}
	}

	return &(string[MAX_CONSOLE_BUFFER]);
}

const char* GeetCount(unsigned int* count)
{
	ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), PREENT_BUFFER, MAX_CONSOLE_BUFFER, count, NULL);

	return PREENT_BUFFER;
	//*count = *count <= 1 ? 0 : *count - 1;
	//PREENT_BUFFER[*count - 1] = '\0';
}

const char* Geet()
{
	unsigned int count;
	return GeetCount(&count);
}
