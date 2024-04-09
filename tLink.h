#ifndef	tLink
#define	tLink
#include <tModules.h>

static Link EmptyLink = {NULL, NULL, false};

void LinkElements(Link* target, Link* prev, Link* next);

void UnLink(Link* target);

Link* RemoveAndNext(Link* target);

#endif