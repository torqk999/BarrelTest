#include <tMutation.h>

#define DEFAULT_POOL_INIT_CAPACITY 128

Mutagen Mutagen_Create(const char* name, BarrelNode** dataLoc, Property** defsLoc, uint count) {
	return
		(Mutagen) {
			name,
			Bucket_Create("COLLECTION", sizeof(COLLECTION), dataLoc, 0, count),
			Bucket_Create("PROPERTY", sizeof(PROPERTY), defsLoc, 0, count)
		};
}
void Mutagen_ctor(Mutagen* loc, const char* name, BarrelNode** dataLoc, Property** defsLoc, uint count) {
	Mutagen tmp = Mutagen_Create(name, dataLoc, defsLoc, count);
	rawTranscribe(loc, &tmp, sizeof(Mutagen));

	for (int i = 0; i < loc->_propData._collection._count; i++) 
		*((BarrelNode**)Bucket_GetPtr(&(loc->_propData), i)) =
			Barrel_Sourced(((Property*)Barrel_GetUnitPtr(&(loc->_propDefs), i))->_type, NULL, 0, DEFAULT_POOL_INIT_CAPACITY);
}

Mutation Mutation_Create(uint offset, uint count)
{
	return
		(Mutation) {
			offset,
			count
		};
}
void Mutation_ctor(Mutation* loc, uint offset, uint count) {
	Mutation tmp = Mutation_Create(loc, offset, count);
	rawTranscribe(loc, &tmp, sizeof(Mutation));
}

Mutable GenePool_Create(Mutation* mutations, Mutagen* mutagens, uint mutCount, uint genCount) {
	return
		(Mutable) {
			Bucket_Create("Mutagen", sizeof(Mutagen), NULL, 0, genCount),
			Bucket_Create("Mutation", sizeof(Mutation), NULL, 0, mutCount),
			Barrel_ctor("uint", sizeof(uint), NULL, 0, DEFAULT_POOL_INIT_CAPACITY),
			Barrel_ctor("uint", sizeof(uint), NULL, 0, DEFAULT_POOL_INIT_CAPACITY)
		};
}
void GenePool_ctor(Mutable* poolLoc, Mutation* mutations, Mutagen* mutagens, uint mutCount, uint genCount) {

	Mutable tmp = GenePool_Create(mutations, mutagens, genCount, mutCount);
	rawTranscribe(poolLoc, &tmp, sizeof(Mutable));

	
}

bool GenePool_GetMutant(uint mutationMask)
{
	return false;
}


/*


	
	00001
	00010
	00011
	00100
	00101
	00110
	00111
	01000
	01001
	01010
	01011
	01100
	01101
	01110
	01111
	10000

*/