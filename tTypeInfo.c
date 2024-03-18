#include <tTypeInfo.h>

bool TypeID_Compare(Request request)
{
	TypeInfo* readType = (TypeInfo*)(request._src);
	TypeInfo* writeType = (TypeInfo*)(request._trg);

	switch (request._type)
	{
	case COMPARE_COMPATIBILITY_FULL:
		if (readType->_name != writeType->_name)
			return false;

	case COMPARE_COMPATIBILITY_WRITE_ALLOWED:
		if (writeType->_flags & READ_ONLY)
			return false;

	case COMPARE_COMPATIBILITY_SIZE:
		if (readType->_size != writeType->_size)
			return false;

		return true;

	default:
		return false;
	}
}

bool TypeID_GetNullValue(TypeInfo* type, void* writeLoc)
{
	for (size_t i = 0; i < type->_size; i++)
		*((char*)writeLoc) = !(type->_nullLoc) ? 0 : *((char*)(type->_nullLoc));

	return type->_nullLoc != 0;
}

void Type_AppendQualifiers(const char* name, uint* flagLoc)
{
	if (findSubString(name, "*"))
		*flagLoc |= POINTER;

	if (findSubString(name, "const"))
		*flagLoc |= READ_ONLY;

	if (findSubString(name, "static"))
		*flagLoc |= STATIC;

	if (findSubString(name, "volatile"))
		*flagLoc |= VOLATILE;

	if (findSubString(name, "unsigned"))
		*flagLoc |= UNSIGNED;

	if (findSubString(name, "float") ||
		findSubString(name, "double"))
		*flagLoc |= FLOATING;

	else if
		(findSubString(name, "int") ||
			findSubString(name, "long") ||
			findSubString(name, "short"))
		*flagLoc |= INTEGRAL;

	else if
		(findSubString(name, "char"))
		*flagLoc |= ALPHA;
}

inline TypeRaw Type_CreateRaw(size_t size, const char* name) { return (TypeRaw) { size, name }; }

TypeInfo Type_CreateNullableInfo(TypeRaw raw, void* nullLoc, uint flags)
{
	Type_AppendQualifiers(raw._name, &flags);
	return (TypeInfo) {raw, nullLoc, flags};
}

TypeInfo Type_CreateInfo(TypeRaw raw, uint flags)
{
	return Type_CreateNullableInfo(raw, NULL, flags);
}

TypeInfo CreateBarrelID(const size_t size, const char* name, void* nullLoc, TypeFlags initFlags)
{
	int unitsPerBlock = 1;
	int barrelsPerBlock = 1;
	
	// Expand for Barrel size constants
	{ 

		// block is the smallest contiguous chunk of memory bounded units of both barrels
		// and vector units set for vector unit size equals sizeof Barrel.
		// check less or greater conditions, adjust units per 'block' accordingly

		if (size < sizeof(Barrel))
		{
			while ((barrelsPerBlock * sizeof(Barrel)) % size)
				barrelsPerBlock++;

			unitsPerBlock = (barrelsPerBlock * sizeof(Barrel)) / size;
		}

		else if (size > sizeof(Barrel))
		{
			while ((unitsPerBlock * size) % sizeof(Barrel))
				unitsPerBlock++;

			barrelsPerBlock = (unitsPerBlock * size) / sizeof(Barrel);
		}
	}

	TypeInfo newTypeID = { size, name, CreateTypeFlags(name, initFlags), nullLoc , unitsPerBlock, barrelsPerBlock };

	return newTypeID;
}

void CloneTypeID(TypeInfo* trg, TypeInfo* src) {
	rawTranscribe(src, trg, sizeof(TypeInfo));
}