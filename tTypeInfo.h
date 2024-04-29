#ifndef	tTypeInfo
#define	tTypeInfo
#include <tHelpers.h>

#define testTypeBinCount 32
#define testPropertyBinCount 64
//#define testTypeBinCount 32
//TypeInfo testTypeBin[testTypeBinCount];
//uint testTypeBinCurrentCount;

bool TypeInfo_Compare(TypeInfo* a, TypeInfo* b);

//bool TypeID_GetNullValue(TypeInfo* type, void* writeLoc);
//bool TypeID_CheckFlags(TypeInfo* info, uint flags);
//uint Type_BuildFlags(const char* name, ClassFlag classFlag);
//inline TypeRaw Type_CreateRaw(size_t size, const char* name);

TypeInfo* TypeInfo_GetNullable(const char* name, size_t size, void* nullLoc);

//TypeInfo Type_CreateInfo(TypeRaw raw, ClassFlag classFlag);

bool TypeInfo_GetInfo(REQUEST request);

Property* Property_Get(const char* name, TypeInfo* info);
TypeInfo* TypeInfo_Get(const char* name, size_t size);

#define TYPE_INFO(typeName) TypeInfo_Get(#typeName, sizeof(typeName))
#define NULL_TYPE_INFO(typeName, nullLoc) TypeInfo_GetNullable(#typeName, sizeof(typeName), nullLoc)
#define PROP_INFO(propName, typeName) Property_Get(#propName, TypeInfo_Get(#typeName, sizeof(typeName)))
#define NULL_PROP_INFO(propName, typeName, nullLoc) Property_Get(#propName, TypeInfo_GetNullable(#typeName, sizeof(typeName), nullLoc))

////////// Public Section /////////////////

const char* TypeInfo_Name(COLLECTION collection);
size_t TypeInfo_Size(COLLECTION collection);

#endif