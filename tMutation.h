#ifndef tMutation
#define tMutation
#include <tBarrel.h>
#include <tBucket.h>
#include <tUserCollection.h>

Mutagen Mutagen_Create(const char* name, BarrelNode** dataLoc, Property** defsLoc, uint count);
void Mutagen_ctor(Mutagen* loc, const char* name, BarrelNode** dataLoc, Property** defsLoc, uint count);

Mutation Mutation_Create(uint offset, uint count);
void Mutation_ctor(Mutation* loc, uint offset, uint count);

GenePool GenePool_Create(Mutation* mutations, Mutagen* mutagens, uint mutCount, uint genCount);
void GenePool_ctor(GenePool* poolLoc, Mutation* mutations, Mutagen* mutagens, uint mutCount, uint genCount);

#define MUTAGEN(name, ...) Mutagen_Create(name, (BarrelNode*[]){ __VA_ARGS__ }, (Property*[]){ __VA_ARGS__ }, PARAM_COUNT( __VA_ARGS__ ) )
#define GENE_POOL(count, ...) GenePool_Create((Mutation[1 << count]){0},(Mutagen[]){ __VA_ARGS__ },1 << count, count )

#endif