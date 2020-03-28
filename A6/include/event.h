#ifndef EVENT_H
#define EVENT_H

#include <float.h>

typedef struct eventS
{
	// uses static vars only and parameter needs to be passed through string 
	float (* eventFunction)(char *);
	float startTime;
} event;

#endif
