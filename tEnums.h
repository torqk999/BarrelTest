#pragma once

enum Constants {
	//NONE = -2,
	//OMEGA = -1,

	//Vector_Default_Capacity = 16,
	//Barrel_Max_Capacity = 1024,
	Hash_Min_Threshold = 30,
	Hash_Default_Threshold = 70,
	Hash_Max_Threshold = 80,
	Hash_Max_Conversion = 64,
};

typedef enum {
	OFF = 0,
	RUN = 1,
	req_ROLL = 8,
	req_FREE = 16,
	wait_ROLL = 32,
	wait_FREE = 64
} BarrelFlags;

typedef enum {
	READ_ONLY = 1,
	FIXED_SIZE = 2,
	STATIC = 4,
	VOLATILE = 8,
	MANAGED = 16,

	// Formatters. Remove after read (f &= !(e))
	EMPTY = 32,
	FILL = 64
} MemFlag;

typedef enum {
	ALPHA = 1,
	INTEGRAL = 2,
	FLOATING = 4,
	TYPE_DEF = 8,
	POINTER = 16,
	UNSIGNED = 32,
	CONST = 64,
} TypeFlag;

typedef enum {
	BUCKET,
	BARREL,
	LIST,
} ClassFlag;

typedef enum {

	// Main
	NONE = -2,
	OMEGA = -1,

	// Generic
	tVARIANT = 0,
	tTRG = 1,

	// Transcribe
	tSRC = 2,
	Ix_TRG = 3,
	Ix_SRC = 4,
	tDIRECTION = 5,

	// Info
	tSIZE = 6,
	tCOUNT = 7,
	tCAPACITY = 8,
	tNAME = 9,

} ParamType;

typedef enum {
	tWRITE,
	tREAD,
	tRAW,
	tCOLLECTION,
	tPOINTER,
	tCHUNK,

	DONE,
	tUSE,
	FREE,
	tRELEASE,

	CREATE_NODE_BARREL,

	EQUIVALENCE,
	COMPATIBILITY_FULL,
	COMPATIBILITY_WRITE_ALLOWED,
	COMPATIBILITY_SIZE,

	INSERT,
	REMOVE_AT,
	REMOVE_FIRST_FOUND,
	DELTA_SIZE,
	DELTA_COUNT,
	tRESIZE,

	__c = 0,
	__s,
	__L,
	__I,
	__x,
	__l,
	__O,
	__b,
	__h,
	__i,
	__o,
	__f,
	__d
} ParamOption;

typedef enum {
	FLAGS,
	ITERATE,
	LOCATION,
	INIT,
	INFO,
	COMPARE,
	CREATE,
	DESTROY,
	MANAGE,
	MODIFY,
	TRANSCRIBE
} RequestType;

