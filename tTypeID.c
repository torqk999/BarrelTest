#include <tTypeID.h>

bool TypeID_Compare(Request request)
{
	TypeID* readType = (TypeID*)(request._src);
	TypeID* writeType = (TypeID*)(request._trg);

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

bool TypeID_GetNullValue(TypeID* type, void* writeLoc)
{
	for (size_t i = 0; i < type->_size; i++)
		*((char*)writeLoc) = !(type->_nullLoc) ? 0 : *((char*)(type->_nullLoc));

	return type->_nullLoc != 0;
}

TypeFlags CreateTypeFlags(const char* name, TypeFlags init)
{
	TypeFlags newFlags = init;

	if (findSubString(name, "*"))
		newFlags |= POINTER;

	if (findSubString(name, "const"))
		newFlags |= READ_ONLY;

	if (findSubString(name, "static"))
		newFlags |= STATIC;

	if (findSubString(name, "volatile"))
		newFlags |= VOLATILE;

	if (findSubString(name, "unsigned"))
		newFlags |= UNSIGNED;

	if (findSubString(name, "float") ||
		findSubString(name, "double"))
		newFlags |= FLOATING;

	else if
		(findSubString(name, "int") ||
			findSubString(name, "long") ||
			findSubString(name, "short"))
		newFlags |= INTEGRAL;

	else if
		(findSubString(name, "char"))
		newFlags |= ALPHA;

	return newFlags;
}

TypeID CreateBarrelID(const size_t size, const char* name, void* nullLoc, TypeFlags initFlags)
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

	TypeID newTypeID = { size, name, CreateTypeFlags(name, initFlags), nullLoc , unitsPerBlock, barrelsPerBlock };

	return newTypeID;
}

void CloneTypeID(TypeID* trg, TypeID* src) {
	rawTranscribe(src, trg, sizeof(TypeID));
}