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
	//POINTER = 1,
	READ_ONLY = 1,
	FIXED_SIZE = 2,
	STATIC = 4,
	VOLATILE = 8,

	UNSIGNED = 16,
	ALPHA = 32,
	INTEGRAL = 64,
	FLOATING = 128,
	STRUCT = 256,
	UNION = 512,

	MANAGED = 1024,
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

	DONE,
	USE,
	FREE,
	RELEASE,

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
	tNAME = 2,
	tSIZE = 3,
	tCOUNT = 4,
	tCAPACITY = 5,


	
	PROVIDE,
	
	



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