#pragma once

#define true 1
#define false 0
#define bool int

#define uint unsigned int
#define ulong unsigned long
#define ullong unsigned long long
#define llong long long

#define STR(x) #x
#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define MCLN( x ) MACRO_CONCAT( x, __LINE__ )
#define MCCN( x ) MACRO_CONCAT( x, __COUNTER__ )

#define TERMINATE "nullTerm"
#define TARG_LINK "targLink"
#define QUE_SIZE 16

#define MaxThreadCount 8
#define MaxPageCount 64
#define PageSize 4096
#define PageThreshold 2048

const char* Reserved_Types[] = {
	TERMINATE,
	TARG_LINK
};
