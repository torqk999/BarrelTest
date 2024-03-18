#pragma once
#include <tModules.h>

Request Transcribe(
	RequestType type,
	void* trg,
	void* src,
	void* buf,
	int tIx,
	int sIx,
	int cnt,
	size_t size
);

Request CreateBarrelNode(BarrelNode* trg);

Request DeltaSizeCapacity(Collection* trg,  int delta);

Request ResizeCollection(Collection* trg, int newSize);
