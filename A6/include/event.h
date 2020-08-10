#ifndef EVENT_H
#define EVENT_H

#include "def.h"

typedef struct eventS
{
	/* 
	uses static vars only and parameter needs to be passed through string 
	needs access to environemnt to do appropriate changes, extern variable variable
	*/
	simTime (* eventFunction)(char **);
	int argCount;
	char ** argVector;
} event;

event * createEvent(simTime (* eveFunction)(char **), int argC, char ** argV);
int freeMemEvent(event * e);

simTime serveCustomer(char** argV);
simTime searchCustomer(char** argV);

typedef struct timedEventS
{
	event e;
	simTime startTime;
} timedEvent;

#endif
