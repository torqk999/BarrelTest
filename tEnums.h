#pragma once

enum Constants {
	NONE = -2,
	OMEGA = -1,

	Vector_Default_Capacity = 16,
	Barrel_Max_Capacity = 1024,
	Hash_Min_Threshold = 30,
	Hash_Default_Threshold = 70,
	Hash_Max_Threshold = 80,
	Hash_Max_Conversion = 64,
	//Heap_Size_Page = 4096,
	//Heap_Page_Max = 128,
	//Barrel_Size_Unit = 32,
	//Barrel_Count_Node = 3,
	//Heap_Threshold = 2048
};

typedef enum {
	DONE = 0,
	RUN = 1,
	req_INIT = 2,
	req_ROLL = 4,
	req_FREE = 8,
	wait_ROLL = 16,
	wait_FREE = 32
} BarrelFlags;

typedef enum {
	NULL = 0,
	POINTER = 1,
	UNSIGNED = 2,
	BARREL = 4,
	READ_ONLY = 8,
	STATIC = 16,
	VOLATILE = 32,
	ALPHA = 64,
	INTEGRAL = 128,
	FLOATING = 256,
	FLEXIBLE = 512,
	ROLLING = 1024
} TypeFlags;

typedef enum {
	ITERATE,

	COMPARE_EQUIVALENCE,
	COMPARE_COMPATIBILITY_FULL,
	COMPARE_COMPATIBILITY_WRITE_ALLOWED,
	COMPARE_COMPATIBILITY_SIZE,

	TRANSCRIBE_RAW_TO_RAW,
	TRANSCRIBE_COLLECTIONS,
	TRANSCRIBE_RAW_TO_COLLECTION,
	TRANSCRIBE_COLLECTION_TO_RAW,

	MODIFY_INSERT,
	MODIFY_REMOVE_AT,
	MODIFY_REMOVE_FIRST_FOUND,
	MODIFY_RESIZE,
} CheckType;