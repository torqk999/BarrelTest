#include <tRequest.h>

Request Transcribe(
	RequestType type,
	void*		trg,
	void*		src,
	void*		buf,
	int			tIx,
	int			sIx,
	int			cnt,
	size_t size
) {
	return (Request) {
			type,
			trg,
			src,
			tIx,
			sIx,
			cnt,
			size
	};
}

Request CreateBarrelNode(BarrelNode* trg) {
	return (Request) {
		CREATE_NODE_BARREL,
			trg,
			NULL,
			0,
			0,
			0,
			0
	};
}

Request DeltaSizeCapacity(Collection* trg, int delta) {
	return (Request) {
		MODIFY_DELTA_CAPACITY,
			trg,
			NULL,
			0,
			0,
			delta,
			0
	};
}

Request ResizeCollection(Collection* trg, int newSize) {
	return DeltaSizeCapacity(trg, newSize - trg->_capacity);
}

Request Decon(Collection* trg) {
	return (Request) {
		MODIFY_DECON,
			trg,
			NULL,
			0,
			0,
			0,
			0
	};
}