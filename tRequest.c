#include <tRequest.h>

Request Transcribe(
	RequestType type,
	void* trg,
	void* src,
	void* buf,
	int			tIx,
	int			sIx,
	size_t size
) {
	return (Request) {
		type,
			trg,
			src,
			tIx,
			sIx,
			size
	};
}

Request InitBarrelNode(BarrelNode* trg, BarrelNode* initiator) {
	return (Request) {
		INIT,
			trg,
			initiator,
			0,
			0,
			0
	};
}

Request DeltaSizeCapacity(COLLECTION trg, void* src, int delta) {
	return (Request) {
		MODIFY_DELTA_CAPACITY,
			trg,
			src,
			0,
			0,
			delta
	};
}

Request ResizeCollection(COLLECTION trg, void* src, int newSize) {
	return DeltaSizeCapacity(trg, src, newSize - trg->_capacity);
}

Request UseCollection(COLLECTION trg)
{
	return (Request) {
		COLLECTION_USE,
			trg,
			NULL,
			0,
			0,
			0,
	};
}

Request FreeCollection(COLLECTION trg)
{
	return (Request) {
		COLLECTION_FREE,
			trg,
			NULL,
			0,
			0,
			0,
	};
}

Request PointCollection(COLLECTION trg)
{
	return (Request) {
		COLLECTION_POINT,
			trg,
			NULL,
			0,
			0,
			0,
	};
}

Request ReleaseCollection(COLLECTION trg)
{
	return (Request) {
		COLLECTION_RELEASE,
			trg,
			NULL,
			0,
			0,
			0,
	};
}

Request Construct(size_t size, const char* typeName, void* src, uint capacity)
{
	return (Request) {
		INIT,
			typeName,
			src,
			0,
			capacity,
			size
	};
}

Request DeConstruct(COLLECTION trg) {
	return (Request) {
		DECON,
			trg,
			NULL,
			0,
			0,
			0
	};
}

LinkedRequest Linked(Request request)
{
	return (LinkedRequest) { (Link) { NULL, NULL, }, request };
}
