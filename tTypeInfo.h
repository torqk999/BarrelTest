#ifndef	tTypeInfo
#define	tTypeInfo
#include <tHelpers.h>

#define testTypeBinCount 32
TypeInfo testTypeBin[testTypeBinCount];
uint testTypeBinCurrentCount = 0;

bool TypeID_Compare(Request request);

bool TypeID_GetNullValue(TypeInfo* type, void* writeLoc);

bool TypeID_CheckFlags(TypeInfo* info, uint flags);

uint Type_BuildFlags(const char* name, ClassFlag classFlag);

inline TypeRaw Type_CreateRaw(size_t size, const char* name);

TypeInfo Type_CreateNullableInfo(TypeRaw raw, ClassFlag classFlag, void* nullLoc);

TypeInfo Type_CreateInfo(TypeRaw raw, ClassFlag classFlag);

TypeInfo* Type_GetInfo(TypeRaw raw, ClassFlag classFlag);

#define TYPE_RAW(typeName) Type_CreateRaw(sizeof(typeName), #typeName)

#define TYPE_ID(typeName, classFlag) Type_GetInfo(Type_CreateRaw(sizeof(typeName), #typeName), classFlag)
#define TYPE_DECL(typeName, nullLoc, flags) const TypeInfo typeName##_TypeID = {sizeof(typeName), #typeName, nullLoc, flags}
#define BARREL_ID(typeName, flags) CreateBarrelID( sizeof(typeName) , #typeName, NULL, flags | FLEXIBLE)

#endif