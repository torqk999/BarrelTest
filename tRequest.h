#ifndef	tRequest
#define	tRequest
#include <tLink.h>

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

Request InitBarrelNode(BarrelNode* trg);

Request DeltaSizeCapacity(COLLECTION trg, void* src, int delta);

Request ResizeCollection(COLLECTION trg, void* src, int newSize);

Request UseCollection(COLLECTION trg);

Request FreeCollection(COLLECTION trg);

Request PointCollection(COLLECTION trg);

Request ReleaseCollection(COLLECTION trg);

Request Construct(size_t size, const char* typeName, void* src, uint capacity);

Request DeConstruct(COLLECTION trg);

LinkedRequest Linked(Request request);

#endif