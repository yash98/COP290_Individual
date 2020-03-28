#ifndef EVENT_H
#define EVENT_H

#include "def.h"

typedef struct eventS
{
	// uses static vars only and parameter needs to be passed through string 
	float (* eventFunction)(char *);
} event;

typedef struct timedEventS
{
	event e;
	time startTime;
} timedEvent;

#endif
