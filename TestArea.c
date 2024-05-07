//#include <tBarrel.h>
#include <tUserCollection.h>
#include <BarrelTest.h>
#include <MutationTest.h>
//#include <tHashMap.h>




//#define Derp(x) _Generic

//Bucket* foo(void** resources) {
//	const char* name = resources[0];
//	size_t unitSize = resources[1];
//	Bucket* bucket = resources[2];
//	void* src = resources[3];
//	const char* p1 = resources[4];
//	const char* p2 = resources[5];
//
//	Bucket* output = Bucket_ctor(name, unitSize, bucket, src, 0, paramCount(p1, p2));
//	return output;
//}
//
//#define FOO(typeName, ...) foo((void*[]){ #typeName, sizeof(typeName), &(Bucket){0}, (typeName[]){ __VA_ARGS__ }, (char[]){ __VA_ARGS__ , 0 }, (char[]){ __VA_ARGS__ , 1}   })


typedef struct {
	void* ptr;
	char myChar;
} MyStruct;

int main() {

	int a = 0b11111;
	int b = a >> 3;
	b = b << 3;

	PREENT_SAFE("% %\n", fmt_b(a), fmt_b(b));

	return 0;

	mutationTest();
	//barrelTest();

	return 0;

	int output = 0;

	COLLECTION myBarrel = Barrel_List(int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

	if (!Collection_Read(myBarrel, &output, 3))
		return 1;

	PREENT_SAFE("value at index 3 of myBarrel: %\n", fmt_i(output));

	COLLECTION myBucket = Bucket_List(int, 0, 1.0f, 2, 3, 4, 5);

	COLLECTION emptyBucket = Bucket_Empty(int, 6);

	if (!Collection_Read(myBucket, &output, 3))
		return 1;

	PREENT_SAFE("collection count of emptyBucket: %\n", fmt_i(Collection_Count(emptyBucket)));
	PREENT_SAFE("value at index 3 of myBucket: %\n", fmt_i(output));

	return 0;
}




/*

Instruction - WORD SIZE
Address
Value - WORD SIZE

PROCESS 
///////////////
READ-ONLY 0xFF...

STACK
|
V

/\/\/\/\/\/\ <- OS dependant

^
|
HEAP 0x00...
////////////////

*/