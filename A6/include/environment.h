#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "queue.h"
#include "actor.h"

typedef struct environmentS
{
	simTime clock;

	int numQueues;
	simTime avgTellerServiceTime;
	teller * tellers;

	fifoQueue * startQueues;
	fifoQueue * endQueues;
	simTime * queueBusyTime; 

	int customerCount;
	customer * customers;

	simTime totalTime;
} environment;

void createEnv(environment * e, int numberQueues, int avgTellerServiceTime, int numCustomer, simTime totalTime);

#endif
