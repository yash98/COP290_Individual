#ifndef EVENT_H
#define EVENT_H

#include "def.h"

typedef struct eventS
{
	/* 
	uses static vars only and parameter needs to be passed through string 
	needs access to environemnt to do appropriate changes, extern variable variable
	*/
	time (* eventFunction)(char **);
	int argCount;
	char ** argVector;
} event;

event * createEvent(time (* eveFunction)(char **), int argC, char ** argV);

time serveCustomer(char** argV);
time searchCustomer(char** argV);

typedef struct timedEventS
{
	event e;
	time startTime;
} timedEvent;

#endif
