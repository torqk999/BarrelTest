#pragma once
#include <tModules.h>

inline bool Collection_Request(COLLECTION collection);
inline bool Collection_Release(COLLECTION collection);

void* Collection_Index(COLLECTION trg, unsigned int index);
uint Collection_Capacity(COLLECTION collection);
uint Collection_Count(COLLECTION collection);

bool Collection_Resize(COLLECTION trg, unsigned int count);
bool Collection_ReadOnly(COLLECTION collection);
bool Collection_ReadSpan(COLLECTION src, void* trg, unsigned int start, unsigned int count);
bool Collection_Read(COLLECTION src, void* trg, unsigned int index);
bool Collection_WriteSpan(COLLECTION trg, void* src, unsigned int start, unsigned int count);
bool Collection_Write(COLLECTION trg, void* src, unsigned int index);
bool Collection_Transcribe(COLLECTION trg, COLLECTION src, unsigned int trgStart, unsigned int srcStart, unsigned int count);
bool Collection_Insertion(COLLECTION trg, COLLECTION src, unsigned int trgStart, unsigned int srcStart, unsigned int count);
bool Collection_Collapse(COLLECTION trg, unsigned int start, unsigned int count);

COLLECTION Bucket_ctor(const char* name, size_t unitSize, void* loc, void* src, int memFlags, uint count);

#define bucket_custom(typeName, flags, ...) Bucket_ctor(#typeName, sizeof(tyeName), (Bucket){0},(typeName[]){__VA_ARGS__}, flags, PARAM_COUNT( __VA_ARGS__ ))
#define bucket(typeName, ...) bucket_custom(typeName, 0, __VA_ARGS__)

#define Bucket_List(typeName, ...) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[]){ __VA_ARGS__ }, 0, PARAM_COUNT( __VA_ARGS__ ) )
#define Bucket_Empty(typeName, capacity) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[capacity]){ 0 }, EMPTY, capacity )
#define Bucket_Fill(typeName, capacity, fill) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, (typeName[capacity]){ fill }, FILL, capacity )
#define Bucket_Inject(typeName, capacity, inject) Bucket_ctor(#typeName, sizeof(typeName), &(Bucket){0}, inject, 0, capacity )

#define Barrel_List(typeName, ...) Barrel_ctor(#typeName, sizeof(typeName), (typeName[]){ __VA_ARGS__ }, 0, PARAM_COUNT( __VA_ARGS__ ) )
#define Barrel_Empty(typeName, capacity) Barrel_ctor(#typeName, sizeof(typeName), (typeName[capacity]){ 0 }, EMPTY, capacity )
#define Barrel_Fill(typeName, capacity, fill) Barrel_ctor(#typeName, sizeof(typeName), (typeName[capacity]){ fill }, FILL, capacity )
#define Barrel_Inject(typeName, capacity, inject) Barrel_ctor(#typeName, sizeof(typeName), inject , 0, capacity )
