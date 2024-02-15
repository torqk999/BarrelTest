#pragma once
#include <tModules.h>

int CreateTypeFlag(const char* name)
{
	return 0;
}

TypeID CreateTypeID(const size_t size, const char* name)
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

	TypeID newTypeID = { size, name, CreateTypeFlag(name), unitsPerBlock, barrelsPerBlock};
	return newTypeID;
}

#define TYPE_ID(typeName) CreateTypeID( sizeof(typeName) , #typeName )