#pragma once
#include <tModules.h>

bool barrelNode_GetIndexLocation(barrel_node* node, uint index, void** location)
{
	uint unitCapacity = node->_blockCount * node->_unitsPerBlock;

	if (index >= unitCapacity)
		return false;

	uint modifiedIndex =  ((node->_blockOffset * node->_unitsPerBlock) + index) % unitCapacity;
	*location = &(((char*)node->_vector._bucket)[modifiedIndex * node->_vector._type._size]);

	return true;
}