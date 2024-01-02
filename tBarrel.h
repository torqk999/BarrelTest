#pragma once
#include <tModules.h>

enum testFlags
{
	RUN = 1,
	DONE = 2,
	req_ROLL = 4,
	req_FREE = 8,
	wait_ROLL = 16,
	wait_FREE = 32
};

enum testConsts
{
	BarrelTestCount = 10,
	DeltaBufferCount = 10
};


void barrelTestRoll(BarrelNode* boop, int delta, int dir)
{
	// dir: -1 = left 1 = right

	if (dir == 0)
		return;

	int absDelta = delta < 0 ? delta * -1 : delta;

	if (boop->_barrelCount > 0)
	{
		int last = boop->_barrelStart + (boop->_barrelCount - 1);

		// full roll

		if (delta == 0)
		{
			if (dir > 0)
			{
				boop->_data[last + 1] = boop->_data[boop->_barrelStart];
				boop->_blockOffset--;
				boop->_barrelStart++;
			}

			else
			{
				boop->_data[boop->_barrelStart - 1] = boop->_data[last];
				boop->_blockOffset++;
				boop->_barrelStart--;
			}
		}

		// with no offset, the end is guaranteed, no action needed

		//   _ ||||||||||||| _

		else
		{
			// tail end rolling

			if ((delta < 0 && dir < 0) ||
				(delta > 0 && dir > 0))
			{
				if (boop->_blockOffset > 0)

					for (int j = 0; j < absDelta; j++)
					{
						if (dir > 0) // roll forward    ||||||| -> |||||||
							for (int i = last; i >= boop->_barrelStart + boop->_blockOffset; i--)
								boop->_data[i + 1] = boop->_data[i];

						else // roll backward           ||||||| <- |||||||
							for (int i = boop->_barrelStart + (boop->_blockOffset - 1); i < boop->_barrelStart + (boop->_barrelCount - 1); i++)
								boop->_data[i] = boop->_data[i + 1];
					}
			}

			// head end rolling

			else
			{
				for (int j = 0; j < absDelta; j++)
				{
					if (dir > 0) // roll forward    ||||||| -> |||||||
					{
						if (boop->_blockOffset > 0)
							for (int i = boop->_barrelStart + (boop->_blockOffset - 2); i >= boop->_barrelStart; i--)
								boop->_data[i + 1] = boop->_data[i];

						boop->_blockOffset--;
						boop->_barrelStart++;
					}

					else // roll backward           ||||||| <- |||||||
					{
						if (boop->_blockOffset > 0)
							for (int i = boop->_barrelStart; i < boop->_barrelStart + (boop->_blockOffset - 1); i++)
								boop->_data[i] = boop->_data[i + 1];

						boop->_blockOffset++;
						boop->_barrelStart--;
					}
				}
			}
		}
	}

	else if (delta == 0 ||
		(delta < 0 && dir > 0) ||
		(delta > 0 && dir < 0))
		boop->_barrelStart += dir > 0 ? 1 : -1;

	boop->_barrelCount += delta;

	boop->_requests -= delta;

	boop->_barrelCount = boop->_barrelCount < 0 ? 0 : boop->_barrelCount;

	boop->_blockOffset =
		boop->_blockOffset < 0 ?
		boop->_barrelCount > boop->_blockOffset * -1 ? (boop->_blockOffset + boop->_barrelCount) % boop->_barrelCount : 0 :
		boop->_blockOffset > boop->_barrelCount ?
		boop->_barrelCount > 0 ? boop->_blockOffset % boop->_barrelCount : 0 :
		boop->_blockOffset;
}

DWORD WINAPI barrelTestWork(void* target)
{
	if (!target)
		return 1;

	BarrelToken* boop = target;

	while (boop->_flags & RUN)
	{
		int delta = boop->_requests < 0 ? -1 : boop->_requests > 0 ? 1 : 0;

		if (boop->_flags & req_ROLL)
		{
			// take this opportunity to shrink the barrel in place by 1 and allow the immediate roll forward
			if (boop->_requests < 0)
			{
				barrelTestRoll(boop, -1, 1);
				boop->_flags &= ~req_ROLL;
			}

			// check for last index for automatic rolling rights
			else if (boop->_index + 1 == BarrelTestCount)
			{
				barrelTestRoll(boop, boop->_requests, 1);
				boop->_flags &= ~req_ROLL;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(boop->_flags & wait_ROLL))
			{
				boop->_bucket[boop->_index + 1]._flags |= req_ROLL;
				boop->_flags |= wait_ROLL;
			}

			else if (boop->_flags & wait_ROLL &&
				!(boop->_bucket[boop->_index + 1]._flags & req_ROLL))
			{
				barrelTestRoll(boop, delta, 1);
				boop->_flags &= ~wait_ROLL;
				boop->_flags &= ~req_ROLL;
			}
		}

		else if (boop->_flags & req_FREE)
		{
			// take this opportunity to grow the barrel in place by 1 and allow the immediate roll backward
			if (boop->_requests > 0)
			{
				barrelTestRoll(boop, 1, -1);
				boop->_flags &= ~req_FREE;
			}

			// check for last index for automatic rolling rights
			else if (boop->_index + 1 == BarrelTestCount)
			{
				barrelTestRoll(boop, boop->_requests, -1);
				boop->_flags &= ~req_FREE;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(boop->_bucket[boop->_index + 1]._flags & req_FREE))
			{
				barrelTestRoll(boop, delta, -1);
				boop->_bucket[boop->_index + 1]._flags |= req_FREE;
				boop->_flags &= boop->_requests != 0 ? 0xFFFFFFFF : ~req_FREE;
			}
		}

		else if (boop->_requests != 0)
		{
			boop->_flags |= boop->_requests < 0 ? req_FREE : req_ROLL;
		}
	}

	boop->_flags |= DONE;

	return 0;
}

void boop_ctor(BarrelNode* node, int index, BarrelToken token)
{
	//bucket[index]._bucket = bucket;
	node[index]._barrelStart = 0;
	node[index]._barrelCount = 0;
	node[index]._blockOffset = 0;
	node[index]._requests = 0;

	node[index]._index = index;
	//bucket[index]._data = dataHead;
	node[index]._handle = CreateThread(NULL, 0, barrelTestWork, &token, 0, &(node[index]._ID));
	node[index]._flags = RUN;

	if (!(node[index]._handle))
		PREENT("thread failed to execute!\n");
}

bool barrelNode_GetIndexLocation(BarrelNode* node, uint index, void** location)
{
	uint unitCapacity = node->_barrelCount * node->_unitsPerBlock;

	if (index >= unitCapacity)
		return false;

	uint modifiedIndex =  ((node->_blockOffset * node->_unitsPerBlock) + index) % unitCapacity;
	*location = &(((char*)node->_vector._bucket)[modifiedIndex * node->_vector._type._size]);

	return true;
}