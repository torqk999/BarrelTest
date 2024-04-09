#include <tTypeInfo.h>

#define testTypeBinCount 32
TypeInfo testTypeBin[testTypeBinCount];
uint testTypeBinCurrentCount = 0;
Page testNullBin;
void* testNullBinHead = &testNullBin;

//
//bool TypeID_Compare(Request request)
//{
//	TypeInfo* readInfo = (TypeInfo*)(request._src);
//	TypeInfo* writeInfo = (TypeInfo*)(request._trg);
//
//	switch (request._type)
//	{
//
//	case COMPARE_COMPATIBILITY_FULL:
//		if (readInfo->_type._name != writeInfo->_type._name)
//			return false;
//
//	case COMPARE_COMPATIBILITY_WRITE_ALLOWED:
//		if (writeInfo->_flags & READ_ONLY)
//			return false;
//
//	case COMPARE_COMPATIBILITY_SIZE:
//		if (readInfo->_type._size != writeInfo->_type._size)
//			return false;
//
//		return true;
//
//	default:
//		return false;
//	}
//}
//
//bool TypeID_GetNullValue(TypeInfo* info, void* writeLoc)
//{
//	for (size_t i = 0; i < info->_type._size; i++)
//		*((char*)writeLoc) = !(info->_nullLoc) ? 0 : *((char*)(info->_nullLoc));
//
//	return info->_nullLoc != 0;
//}
//
//bool TypeID_CheckFlags(TypeInfo* info, uint flags) {
//	return info->_flags & flags == flags;
//}
//
uint TypeInfo_BuildFlags(const char* name)
{
	uint flags = 0;

	if (findSubString(name, "const") > -1)
		flags |= READ_ONLY | FIXED_SIZE;

	if (findSubString(name, "static") > -1)
		flags |= STATIC;

	if (findSubString(name, "volatile") > -1)
		flags |= VOLATILE;

	if (findSubString(name, "unsigned") > -1)
		flags |= UNSIGNED;

	if (findSubString(name, "float") > -1 ||
		findSubString(name, "double") > -1)
		flags |= FLOATING;

	else if
		(findSubString(name, "int") > -1 ||
			findSubString(name, "long") > -1 ||
			findSubString(name, "short") > -1)
		flags |= INTEGRAL;

	else if
		(findSubString(name, "char") > -1)
		flags |= ALPHA;

	return flags;
}

void TypeInfo_BarrelBlockCounts(size_t size, uint* unitsPerBlock, uint* barrelsPerBlock) {

	// block is the smallest contiguous chunk of memory bounded units of both barrels
	// and vector units set for vector unit size equals sizeof Barrel.
	// check less or greater conditions, adjust units per 'block' accordingly

	*unitsPerBlock = 1;
	*barrelsPerBlock = 1;

	if (size < sizeof(Barrel))
	{
		while ((*barrelsPerBlock * sizeof(Barrel)) % size)
			(*barrelsPerBlock)++;

		*unitsPerBlock = (*barrelsPerBlock * sizeof(Barrel)) / size;
	}

	else if (size > sizeof(Barrel))
	{
		while ((*unitsPerBlock * size) % sizeof(Barrel))
			(*unitsPerBlock)++;

		*barrelsPerBlock = (*unitsPerBlock * size) / sizeof(Barrel);
	}

}

void* TypeInfo_GetNullableTemplate(size_t size, void* nullLoc) {
	if (!nullLoc)
		return NULL;

	size_t endOfPage = &testNullBin + sizeof(Page);
	size_t headLoc = testNullBinHead;
	if (headLoc + size > endOfPage)
		return NULL;

	rawTranscribe(testNullBinHead, nullLoc, size);
	void* ptr = testNullBinHead;
	testNullBinHead = headLoc + size;
	return ptr;
}

TypeInfo* TypeInfo_GetNullable(const char* name, size_t size, void* nullLoc)
{
	if (testTypeBinCurrentCount >= testTypeBinCount)
	{
		PREENT("No type slots available...\n");
		return NULL;
	}

	uint flags = TypeInfo_BuildFlags(name);
	uint unitsPerBlock, barrelsPerBlock;
	TypeInfo_BarrelBlockCounts(size, &unitsPerBlock, &barrelsPerBlock);
	void* nullTmp = TypeInfo_GetNullableTemplate(size, nullLoc);

	TypeInfo newInfo = {
		name,
		size,
		nullTmp,
		unitsPerBlock,
		barrelsPerBlock
	};

	TypeInfo* binLocation = testTypeBin;
	rawTranscribe(&(testTypeBin[testTypeBinCurrentCount]), &newInfo, sizeof(TypeInfo));
	TypeInfo* ptr = &(testTypeBin[testTypeBinCurrentCount]);
	testTypeBinCurrentCount++;
	return ptr;
}

const char* TypeInfo_Name(COLLECTION collection)
{
	return collection->_type->_name;
}

size_t TypeInfo_Size(COLLECTION collection)
{
	return collection->_type->_size;
}

bool TypeInfo_GetInfo(REQUEST request)
{
	TypeInfo* info = request._params[tSRC];

	switch ((ParamType)request._params[tVARIANT]) {
	case tNAME:
		*((char**)request._params[tNAME]) = info->_name;
		return true;

	case  tSIZE:
		*((size_t*)request._params[tSIZE]) = info->_size;
		return true;


	default:
		return false;
	}
}

TypeInfo* TypeInfo_Get(const char* name, size_t size) { return TypeInfo_GetNullable(name, size, NULL); }


