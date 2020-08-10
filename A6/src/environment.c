#include "environment.h"

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

void createEnv(environment * e, int numberQueues, int numTellers, simTime avgTellerServiceTime, int numCustomer, simTime totalTime) {
	e->clock = 0.0;

	e->numQueues = numberQueues;
	e->numTellers = numTellers;
	if (numTellers == numberQueues) {
		e->type = multiQueue;
	} else {
		e->type = singleQueue;
	}

	e->tellers = malloc(numTellers * sizeof(teller));
	e->queueBusyTime = malloc(numTellers * sizeof(simTime));
	e->startQueues = malloc(numberQueues * sizeof(fifoQueue));
	e->endQueues = malloc(numTellers * sizeof(fifoQueue));
	
	for (int i=0; i < numTellers; i++) {
		createTeller(e->tellers + i, i);
		createFQueue(e->endQueues+i);
		fifoQueue * relevantEndQueue = e->endQueues + i;

		// Go on break at start
		char ** createdArgV = malloc(1*sizeof(char*));
		*createdArgV = malloc(7*sizeof(char));
		sprintf(*createdArgV, "%d", i);
		pushFQueue(relevantEndQueue, createNode(createEvent(&searchCustomer, 1, createdArgV)));

		*(e->queueBusyTime + i) = 0.0;
	}

	for (int i = 0; i < numberQueues; i++) {
		createFQueue(e->startQueues+i);
	}

	e->avgTellerServiceTime = avgTellerServiceTime;

	// Uniform Distribution arrivals
	simTime arrivals[numCustomer];

	for (int i = 0; i < numCustomer; i++) {
		arrivals[i] = totalTime * rand() / ((simTime) RAND_MAX);
	}

	// Selection sort array
	for (int i = 0; i < numCustomer; i++) {
		simTime min = 1.79769e+308;
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
