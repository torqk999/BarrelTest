#pragma once
#include <tModules.h>

bool CompareTypes(CollectionRequest request)
{
	TypeID* readType = request._src;
	TypeID* writeType = request._trg;

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

CollectionExtensions CreateCollectionExtensions(TypeFlags flags)
{
	CollectionExtensions newExtensions;

	if (flags & ALPHA)
		newExtensions.Compare = flags & POINTER ? defaultStringCompare : defaultCharCompare;

	if (flags & INTEGRAL)
		newExtensions.Compare = flags & UNSIGNED ? UnsignedIntegralCompare : SignedIntegralCompare;

	if (flags & FLOATING)
		newExtensions.Compare = FloatingCompare;


	if (flags & BARREL)
	{
		newExtensions.Iterate = barrel_Iterate;
		newExtensions.Modify = barrel_Modify;
		newExtensions.Transcribe = barrel_Transcribe;
	}
	else
	{
		newExtensions.Iterate = Bucket_Iterate;
		newExtensions.Modify = Bucket_Modify;
		newExtensions.Transcribe = Bucket_Transcribe;
	}
		
}

TypeID CreateTypeID(const size_t size, const char* name, TypeFlags initFlags)
{
	// block is the smallest contiguous chunk of memory bounded units of both barrels
	// and vector units set for vector unit size equals sizeof Barrel

	int unitsPerBlock = 1;
	int barrelsPerBlock = 1;

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

	TypeFlags typeFlags = CreateTypeFlags(name, initFlags);
	CollectionExtensions extensions = CreateCollectionExtensions(typeFlags);

	TypeID newTypeID = { size, name, typeFlags, extensions, unitsPerBlock, barrelsPerBlock};
	return newTypeID;
}

#define TYPE_ID(typeName) CreateTypeID( sizeof(typeName) , #typeName , 0)
#define BARREL_ID(typeName) CreateTypeID( sizeof(typeName) , #typeName , BARREL)