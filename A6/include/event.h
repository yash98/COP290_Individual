#ifndef EVENT_H
#define EVENT_H

#include "def.h"

typedef struct eventS
{
	/* 
	uses static vars only and parameter needs to be passed through string 
	needs access to environemnt to do appropriate changes, extern variable variable
	*/
	time (* eventFunction)(int, char **);
	int argC;
	char** argV;
} event;

typedef struct timedEventS
{
	event e;
	time startTime;
} timedEvent;

#endif
