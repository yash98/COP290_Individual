#include "environment.h"

#include <malloc.h>
#include <stdlib.h>

void createEnv(environment * e, int numberQueues, int avgTellerServTime, int numCustomer, time totalTime) {
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

	// Uniform Distribution arrivals
	time arrivals[numCustomer];

	for (int i = 0; i < numCustomer; i++) {
		arrivals[i] = totalTime * rand() / ((time) RAND_MAX);
	}

	// Selection sort array
	for (int i = 0; i < numCustomer; i++) {
		//  Arrival times are positive so default value is always smaller
		int min = -1.0;
		int minIndex = i;
		for (int j = i; j < numCustomer; j++) {
			if (arrivals[j] < min) {
				minIndex = j;
				min = arrivals[j];
			}
		}
		arrivals[minIndex] = arrivals[i];
		arrivals[i] = min;
	}

	e->customerCount = numCustomer;
	e->customers = malloc(numCustomer * sizeof(customer));
	for (int i=0; i<numCustomer; i++) {
		createCustomer(e->customers + i, i, arrivals[i]);
	}

	e->totalTime = totalTime;
}
