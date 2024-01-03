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

typedef struct {
	BarrelNode* _barrel;
	unsigned int _index;
} BarrelToken;

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
				_heapHead[last + 1] = _heapHead[boop->_barrelStart];
				boop->_blockOffset--;
				boop->_barrelStart++;
			}

			else
			{
				_heapHead[boop->_barrelStart - 1] = _heapHead[last];
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
								_heapHead[i + 1] = _heapHead[i];

						else // roll backward           ||||||| <- |||||||
							for (int i = boop->_barrelStart + (boop->_blockOffset - 1); i < boop->_barrelStart + (boop->_barrelCount - 1); i++)
								_heapHead[i] = _heapHead[i + 1];
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
								_heapHead[i + 1] = _heapHead[i];

						boop->_blockOffset--;
						boop->_barrelStart++;
					}

					else // roll backward           ||||||| <- |||||||
					{
						if (boop->_blockOffset > 0)
							for (int i = boop->_barrelStart; i < boop->_barrelStart + (boop->_blockOffset - 1); i++)
								_heapHead[i] = _heapHead[i + 1];

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

void barrelRoll(BarrelNode* boop, int delta, int dir)
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
				_heapHead[last + 1] = _heapHead[boop->_barrelStart];
				boop->_blockOffset -= boop->_blockOffset % boop->_barrelsPerBlock ? 0 : 1;
				boop->_barrelStart++;
			}

			else
			{
				_heapHead[boop->_barrelStart - 1] = _heapHead[last];
				boop->_blockOffset += boop->_blockOffset % boop->_barrelsPerBlock ? 0 : 1;
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
								_heapHead[i + 1] = _heapHead[i];

						else // roll backward           ||||||| <- |||||||
							for (int i = boop->_barrelStart + (boop->_blockOffset - 1); i < boop->_barrelStart + (boop->_barrelCount - 1); i++)
								_heapHead[i] = _heapHead[i + 1];
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
								_heapHead[i + 1] = _heapHead[i];

						boop->_blockOffset--;
						boop->_barrelStart++;
					}

					else // roll backward           ||||||| <- |||||||
					{
						if (boop->_blockOffset > 0)
							for (int i = boop->_barrelStart; i < boop->_barrelStart + (boop->_blockOffset - 1); i++)
								_heapHead[i] = _heapHead[i + 1];

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

	BarrelToken token = *(BarrelToken*)target;

	BarrelNode* boop = token._barrel;
	unsigned int index = token._index;

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
			else if (index + 1 == BarrelTestCount)
			{
				barrelTestRoll(boop, boop->_requests, 1);
				boop->_flags &= ~req_ROLL;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(boop->_flags & wait_ROLL))
			{
				_nodeHead[index + 1]._flags |= req_ROLL;
				boop->_flags |= wait_ROLL;
			}

			else if (boop->_flags & wait_ROLL &&
				!(_nodeHead[index + 1]._flags & req_ROLL))
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
			else if (index + 1 == BarrelTestCount)
			{
				barrelTestRoll(boop, boop->_requests, -1);
				boop->_flags &= ~req_FREE;
			}

			// inform the next barrel that there is a requested slot to roll to
			else if (!(_nodeHead[index + 1]._flags & req_FREE))
			{
				barrelTestRoll(boop, delta, -1);
				_nodeHead[index + 1]._flags |= req_FREE;
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

void boop_ctor(
	BarrelToken* token,
	LPHANDLE handleLoc,
	LPDWORD idLoc,
	Vector template,
	int barrelStart)
{
	token->_barrel->_barrelStart = barrelStart;
	token->_barrel->_barrelCount = 0;
	token->_barrel->_blockOffset = 0;
	token->_barrel->_requests = 0;
	token->_barrel->_unitsPerBlock = 1;
	token->_barrel->_barrelsPerBlock = 1;

	handleLoc = CreateThread(NULL, 0, barrelTestWork, token, 0, idLoc);

	if (!handleLoc)
		PREENT("thread failed to execute!\n");
	else
		token->_barrel->_flags = RUN;
}

bool barrelNode_GetIndexLocation(BarrelNode* node, uint index, void** target)
{
	uint unitCapacity = node->_barrelCount * node->_unitsPerBlock;

	if (index >= unitCapacity)
		return false;

	ullong byteLength = node->_barrelCount * Barrel_Size_Unit;
	ullong byteOffset = (node->_blockOffset * node->_barrelsPerBlock) * Barrel_Size_Unit;
	ullong byteIndex = index * node->_vector._type._size;

	// vector offset + barrel offset + heap ptr

	*target = ((byteOffset + byteIndex) % byteLength) + (node->_barrelStart * Barrel_Size_Unit) + _heapHead;

	//Vector_MoveIndex(source, target, 0, 0, node->_vector._type._size);

	return true;
}