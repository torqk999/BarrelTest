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

GenePool GenePool_Create(Mutation* mutations, Mutagen* mutagens, uint mutCount, uint genCount) {
	return
		(GenePool) {
			Bucket_Create("Mutagen", sizeof(Mutagen), NULL, 0, genCount),
			Bucket_Create("Mutation", sizeof(Mutation), NULL, 0, mutCount),
			Barrel_ctor("uint", sizeof(uint), NULL, 0, DEFAULT_POOL_INIT_CAPACITY),
			Barrel_ctor("uint", sizeof(uint), NULL, 0, DEFAULT_POOL_INIT_CAPACITY)
		};
}
void GenePool_ctor(GenePool* poolLoc, Mutation* mutations, Mutagen* mutagens, uint mutCount, uint genCount) {

	GenePool tmp = GenePool_Create(mutations, mutagens, genCount, mutCount);
	rawTranscribe(poolLoc, &tmp, sizeof(GenePool));

	
}