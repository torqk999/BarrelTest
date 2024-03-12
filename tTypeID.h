#pragma once
#include <tHelpers.h>

bool TypeID_Compare(Request request);

bool TypeID_GetNullValue(TypeID* type, void* writeLoc);

TypeFlags CreateTypeFlags(const char* name, TypeFlags init);

TypeID CreateBarrelID(const size_t size, const char* name, void* nullLoc, TypeFlags initFlags);

void CloneTypeID(TypeID* trg, TypeID* src);

#define TYPE_ID(typeName, flags) {sizeof(typeName), #typeName, NULL, flags}
#define TYPE_DECL(typeName, nullLoc, flags) const TypeID typeName##_TypeID = {sizeof(typeName), #typeName, nullLoc, flags}
#define BARREL_ID(typeName, flags) CreateBarrelID( sizeof(typeName) , #typeName, NULL, flags | BARREL)
