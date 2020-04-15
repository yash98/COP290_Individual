#ifndef ACTOR_H
#define ACTOR_H

#include "def.h"
#include "queue.h"

typedef struct tellerS
{
	int tellerId;
	time idleTime;
	time serviceTime;
} teller;

typedef struct customerS
{
	int customerIndex;
	int lineIndex;
	time enterTime;
	time exitTime;
	time waitTime;
	time serviceTime;
} customer;

#endif
