#pragma once
#include <tModules.h>
#include <tPreent.h>

//extern CollectionRequest;

float Clamp(float value, float min, float max);

int ClampInt(int value, int min, int max);

int pow(int base, unsigned int power);

bool defaultCharCompare(CollectionRequest request);

bool defaultStringCompare(CollectionRequest request);

int UnsignedIntegralCompare(CollectionRequest request);

int SignedIntegralCompare(CollectionRequest request);

int FloatingCompare(CollectionRequest request);

int findSubStringCustom(const char* src, const char* sub, bool (*compare)(CollectionRequest request));

int findSubString(const char* src, const char* sub);

unsigned int paramCount(char* rawArray, char* checkArray);

unsigned int paramCount0(const char* params);

void rawTranscribe(void* src, void* trg, size_t size);

void barrelTranscribe(void* src, void* trg, unsigned int count);

void TranscribeElement(CollectionRequest request);

void TranscribeSpan(CollectionRequest request);