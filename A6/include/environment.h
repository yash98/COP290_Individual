#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "queue.h"
#include "actor.h"

typedef struct environmentS
{
	time clock;

	int numQueues;
	teller * tellers;
	fifoQueue * startQueues;
	fifoQueue * endQueues;
	int * queueBusyTime; 
	time avgTellerServeTime;

	int cutomerCount;
	customer * customers;
} environment;


#endif
