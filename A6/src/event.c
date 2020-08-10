#include "event.h"
#include "queue.h"
#include "environment.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern environment * mainEnv;

int freeMemEvent(event * e) {
	for (int i = 0; i < e->argCount; i++) {
		free(e->argVector+i);
	}
	free(e);
	return 1;
}

event * createEvent(simTime (* eveFunction)(char **), int argC, char ** argV) {
	event * returnEvent = malloc(sizeof(event));
	returnEvent->eventFunction = eveFunction;
	returnEvent->argCount = argC;
	returnEvent->argVector = argV;

	return returnEvent;
}

/*
1. Teller Id
2. Customer Id
*/
simTime serveCustomer(char ** argV) {
	char ** argVPtr = argV;
	int tellerId = atoi(*argVPtr);
	argVPtr += 1;
	int customerId = atoi(*argVPtr);

	debugPrintf("%lf: teller %d started serving customer %d\n", mainEnv->clock, tellerId, customerId);

	fifoQueue * relevantQueue = mainEnv->startQueues + tellerId;
	teller * relevantTeller = mainEnv->tellers + tellerId;
	customer * relevantCustomer = mainEnv->customers + customerId;

	simTime timeUtilized = (2 * mainEnv->avgTellerServeTime * rand()) / ((simTime) RAND_MAX);

	relevantTeller->serviceTime += timeUtilized;
	relevantCustomer->serviceTime = timeUtilized;
	relevantCustomer->exitTime = mainEnv->clock + timeUtilized;
	
	// add searching task to endQueue
	fifoQueue * relevantEndQueue = mainEnv->endQueues + tellerId;
	char ** createdArgV = malloc(1*sizeof(char*));
	*createdArgV = malloc(7*sizeof(char));
	strcpy(*(argV), *createdArgV);
	pushFQueue(relevantEndQueue, createNode(createEvent(&searchCustomer, 1, createdArgV)));

	return timeUtilized;
}

/*
1. Teller Id
*/
simTime searchCustomer(char ** argV) {
	char ** argVPtr = argV;
	int tellerId = atoi(*argVPtr);

	fifoQueue * relevantQueue = mainEnv->startQueues + tellerId;
	teller * relevantTeller = mainEnv->tellers + tellerId;

	if (relevantQueue->length > 0) {
		return 0.0;
	}

	// select non empty queues
	int numSelectedQueues = 0;
	fifoQueue ** selectedQueues = malloc(mainEnv->numQueues * sizeof(fifoQueue *));
	fifoQueue ** qPtr = selectedQueues;

	// randomly selecting queue from tellers
	for (int i=0; i < mainEnv->numQueues; i++) {
		fifoQueue * iterQueue = mainEnv->startQueues + i;
		if (i != tellerId) {
			if (relevantQueue->length > 0) {
				*qPtr = relevantQueue;
				qPtr += 1;
				numSelectedQueues += 1;
			}
		}
	}
	if (numSelectedQueues == 0) {		
		free(selectedQueues);
		// Teller taking break
		debugPrintf("%lf: teller %d taking a break for %lf minutes\n", mainEnv->clock, tellerId, relevantTeller->breakDuration);

		// Add searching task that is done at returning
		fifoQueue * relevantEndQueue = mainEnv->endQueues + tellerId;
		char ** createdArgV = malloc(1*sizeof(char*));
		*createdArgV = malloc(7*sizeof(char));
		strcpy(*(argV), *createdArgV);
		pushFQueue(relevantEndQueue, createNode(createEvent(&searchCustomer, 1, createdArgV)));

		return relevantTeller->breakDuration;
	}
		
	int selection = rand()/numSelectedQueues;
	fifoQueue * selectedQueue = mainEnv->startQueues + selection;
	node * takingJob = popFQueue(selectedQueue);

	// taking job from another queue and adding for this teller
	debugPrintf("%lf: teller %d took customer from queue %d\n", mainEnv->clock, tellerId, selection);
	char * copyTellerId = malloc(7 * sizeof(char));
	strcpy(copyTellerId, *argV);
	*(takingJob->eve->argVector) = copyTellerId;
	pushFQueue(relevantQueue, takingJob);
	freeMemEvent(takingJob->eve);

	free(selectedQueues);
	return 0.0;
}