#pragma once
#include <tHelpers.h>

bool TypeID_Compare(Request request);

bool TypeID_GetNullValue(TypeInfo* type, void* writeLoc);

void Type_AppendQualifiers(const char* name, uint* flagLoc);

inline TypeRaw Type_CreateRaw(size_t size, const char* name);

TypeInfo Type_CreateNullableInfo(TypeRaw raw, void* nullLoc, uint flags);

TypeInfo Type_CreateInfo(TypeRaw raw, uint flags);

TypeInfo CreateBarrelID(const size_t size, const char* name, void* nullLoc, TypeFlags initFlags);

void CloneTypeID(TypeInfo* trg, TypeInfo* src);

#define TYPE_RAW(typeName) Type_CreateRaw(sizeof(typeName), #typeName)

#define TYPE_ID(typeName, flags) {sizeof(typeName), #typeName, NULL, flags}
#define TYPE_DECL(typeName, nullLoc, flags) const TypeInfo typeName##_TypeID = {sizeof(typeName), #typeName, nullLoc, flags}
#define BARREL_ID(typeName, flags) CreateBarrelID( sizeof(typeName) , #typeName, NULL, flags | FLEXIBLE)
