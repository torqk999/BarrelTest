#pragma once
#include <tHelpers.h>

static CollectionExtensions GlobalBucketExtensionTemplate;
static CollectionExtensions GlobalBarrelExtensionTemplate;

bool CompareTypes(CollectionRequest request);

TypeFlags CreateTypeFlags(const char* name, TypeFlags init);

CollectionExtensions CreateCollectionExtensions(TypeFlags initFlags, CollectionExtensions(*TemplateExtension)());

TypeID CreateTypeID(const size_t size, const char* name, TypeFlags initFlags, CollectionExtensions(*TemplateExtension)());

#define BUCKET_ID(typeName, flags) CreateTypeID( sizeof(typeName) , #typeName , flags | 0, Bucket_TemplateExtension)
#define BARREL_ID(typeName, flags) CreateTypeID( sizeof(typeName) , #typeName , flags | BARREL, barrel_TemplateExtension)
