#ifndef ACTOR_H
#define ACTOR_H

#include "def.h"
#include "queue.h"

typedef struct teller
{
	int tellerId;
	fifoQueue history;
};

typedef struct customer
{
	int customerIndex;
	int lineIndex;
	time enterTime;
	time exitTime;
	time serviceTime;
};

#endif
