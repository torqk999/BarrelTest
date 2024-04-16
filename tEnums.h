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
	UNSIGNED = 1,
	ALPHA = 2,
	INTEGRAL = 4,
	FLOATING = 8,
	STRUCT = 16,
	UNION = 32
} TypeFlag;

typedef enum {
	BUCKET,
	BARREL,
	LIST,
} ClassFlag;

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

typedef enum {

	// Main
	NONE = -2,
	OMEGA = -1,
	FLAG = 1,



	tVARIANT = 0,
	tTRG = 1,

	tREAD,
	tWRITE,
	tRAW,
	tCOLLECTION,
	tPOINTER,
	tCHUNK,

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
	
	PROVIDE,
	
	DONE,
	USE,
	FREE,
	RELEASE,



	CREATE_NODE_BARREL,


	COMPARE_EQUIVALENCE,
	COMPARE_FLAG,
	COMPARE_COMPATIBILITY_FULL,
	COMPARE_COMPATIBILITY_WRITE_ALLOWED,
	COMPARE_COMPATIBILITY_SIZE,

	//TRANSCRIBE,

	RAW_TO_RAW,
	COLLECTIONS,
	RAW_TO_COLLECTION,
	COLLECTION_TO_RAW,

	MODIFY_INSERT,
	MODIFY_REMOVE_AT,
	MODIFY_REMOVE_FIRST_FOUND,
	MODIFY_DELTA_CAPACITY,
	MODIFY_DELTA_COUNT,

	__c,
	__s,
	__L,
	__I,
	__x,
	__l,
	__O,
	__h,
	__i,
	__o,
	__f,
	__d
} ParamType;