#include "environment.h"

#include <malloc.h>

void createEnv(environment * e, int numberQueues, int avgTellerServTime, int numCustomer) {
	e->clock = 0.0;

	e->numQueues = numberQueues;
	e->tellers = malloc(numberQueues * sizeof(teller));
	
	for (int i=0; i < numberQueues; i++) {
		createTeller(e->tellers + i, i);
	}
	
	e->startQueues = malloc(sizeof(fifoQueue));
	createFQueue(e->startQueues);

	e->endQueues = malloc(sizeof(fifoQueue));
	createFQueue(e->endQueues);

	e->queueBusyTime = malloc(numberQueues * sizeof(int));
	for (int i=0; i < numberQueues; i++) {
		*(e->queueBusyTime + i) = 0.0;
	}

	e->avgTellerServeTime = avgTellerServTime;

	e->cutomerCount = numCustomer;
	e->customers = malloc(numCustomer * sizeof(customer));
	for (int i=0; i<numCustomer; i++) {
		createCustomer(e->customers + i, i);
	}
}
