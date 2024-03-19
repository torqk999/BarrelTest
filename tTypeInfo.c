#include <tTypeInfo.h>

bool TypeID_Compare(Request request)
{
	TypeInfo* readInfo = (TypeInfo*)(request._src);
	TypeInfo* writeInfo = (TypeInfo*)(request._trg);

	switch (request._type)
	{
	case COMPARE_COMPATIBILITY_FULL:
		if (readInfo->_type._name != writeInfo->_type._name)
			return false;

	case COMPARE_COMPATIBILITY_WRITE_ALLOWED:
		if (writeInfo->_flags & READ_ONLY)
			return false;

	case COMPARE_COMPATIBILITY_SIZE:
		if (readInfo->_type._size != writeInfo->_type._size)
			return false;

		return true;

	default:
		return false;
	}
}

bool TypeID_GetNullValue(TypeInfo* info, void* writeLoc)
{
	for (size_t i = 0; i < info->_type._size; i++)
		*((char*)writeLoc) = !(info->_nullLoc) ? 0 : *((char*)(info->_nullLoc));

	return info->_nullLoc != 0;
}

uint Type_BuildFlags(const char* name, ClassFlag classFlag)
{
	uint flags = 0;

	switch (classFlag) {
	case BUCKET:
		flags |= FIXED_SIZE;
		break;

	case BARREL:
		flags |= MANAGED;
		break;

	case LIST:
		flags |= MANAGED;
		break;
	}

	if (findSubString(name, "const"))
		flags |= READ_ONLY | FIXED_SIZE;

	if (findSubString(name, "static"))
		flags |= STATIC;

	if (findSubString(name, "volatile"))
		flags |= VOLATILE;

	if (findSubString(name, "unsigned"))
		flags |= UNSIGNED;

	if (findSubString(name, "float") ||
		findSubString(name, "double"))
		flags |= FLOATING;

	else if
		(findSubString(name, "int") ||
			findSubString(name, "long") ||
			findSubString(name, "short"))
		flags |= INTEGRAL;

	else if
		(findSubString(name, "char"))
		flags |= ALPHA;

	return flags;
}

inline TypeRaw Type_CreateRaw(size_t size, const char* name) { return (TypeRaw) { size, name }; }

TypeInfo Type_CreateNullableInfo(TypeRaw raw, ClassFlag classFlag, void* nullLoc)
{
	uint flags = Type_BuildFlags(raw._name, classFlag);
	uint unitsPerBlock, barrelsPerBlock;
	Type_BarrelBlockCounts(raw._size, &unitsPerBlock, &barrelsPerBlock);
	return (TypeInfo) { raw, nullLoc, flags, unitsPerBlock, barrelsPerBlock };
}

TypeInfo Type_CreateInfo(TypeRaw raw, ClassFlag classFlag)
{
	return Type_CreateNullableInfo(raw, classFlag, NULL);
}

TypeInfo* Type_GetInfo(TypeRaw raw, ClassFlag flag) {
	if (testTypeBinCurrentCount >= testTypeBinCount)
	{
		PREENT("No type slots available...\n");
		return NULL;
	}

	TypeInfo tmpInfo = Type_CreateInfo(raw, flag);
	rawTranscribe(&tmpInfo, &(testTypeBin[testTypeBinCurrentCount]), sizeof(TypeInfo));
	TypeInfo* ptr = &(testTypeBin[testTypeBinCurrentCount]);
	testTypeBinCurrentCount++;
	return ptr;
}

void Type_BarrelBlockCounts(size_t size, uint* unitsPerBlock, uint* barrelsPerBlock) {

	// block is the smallest contiguous chunk of memory bounded units of both barrels
	// and vector units set for vector unit size equals sizeof Barrel.
	// check less or greater conditions, adjust units per 'block' accordingly

	if (size < sizeof(Barrel))
	{
		while ((*barrelsPerBlock * sizeof(Barrel)) % size)
			(*barrelsPerBlock)++;

		unitsPerBlock = (*barrelsPerBlock * sizeof(Barrel)) / size;
	}

	else if (size > sizeof(Barrel))
	{
		while ((*unitsPerBlock * size) % sizeof(Barrel))
			(*unitsPerBlock)++;

		barrelsPerBlock = (*unitsPerBlock * size) / sizeof(Barrel);
	}

}
