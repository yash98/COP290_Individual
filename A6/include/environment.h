#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "queue.h"
#include "actor.h"

typedef struct environmentS
{
	int numQueues;
	fifoQueue queues;
	int cutomerCount;
	teller * tellers;
	customer * customers;
} environment;


#endif
