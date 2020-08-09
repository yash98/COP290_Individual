#include "simulator.h"
#include "environment.h"
#include "event.h"

#include <malloc.h>
#include <stdlib.h>

environment * mainEnv;

simTime multiQueueSystem() {
	customer * lastCustomer = mainEnv->customers + mainEnv->customerCount - 1;

	customer * latestCustomer = mainEnv->customers;

	enum mode {customer, teller} actionMode;

	while (lastCustomer->exitTime < 0) {
		if (actionMode == customer) {
			mainEnv->clock = latestCustomer->enterTime;
			// Find the shortest queues
			int minLength = 2147483647;
			for (int i = 0; i < mainEnv->numQueues; i++) {
				if ((mainEnv->startQueues+i)->length < minLength) {
					minLength = (mainEnv->startQueues+i)->length;
				}
			}
			int queueIndexes[mainEnv->numQueues];
			int nextIndex = 0;
			for (int i = 0; i < mainEnv->numQueues; i++) {
				if ((mainEnv->startQueues+i)->length == minLength) {
					queueIndexes[nextIndex] = i;
					nextIndex++;
				}
			}
			// Randomly select a queue
			int chosenQueue = queueIndexes[rand()%nextIndex];
			
			char ** createdArgV = (char **) malloc(2*sizeof(char*));
			*createdArgV = malloc(7*sizeof(char));
			*(createdArgV+1) = malloc(7*sizeof(char));
			sprintf(*createdArgV, "%s", chosenQueue);
			sprintf(*createdArgV+1, "%s", latestCustomer->customerId);

			pushFQueue(mainEnv->startQueues+chosenQueue, createNode(createEvent(&serveCustomer, 2, createdArgV)));
			
			latestCustomer++;
		} else {
			
			// Start Queue
			event * sqEve = popFQueue(mainEnv->startQueues)->eve;
			simTime busyFor = (sqEve->eventFunction) (sqEve->argVector);
		}
	}
}

int main(int argc, char * argv[]) {
	// Cmd Args
	int numCustomers = atoi(argv[1]);
	int numTellers = atoi(argv[2]);
	simTime totalTime = atof(argv[3]);
	simTime avgTellerServeTime = atof(argv[3]);

	environment * multiQueueEnv = (environment *) malloc(sizeof(environment));
	createEnv(multiQueueEnv, numTellers, avgTellerServeTime, numCustomers, totalTime);
	environment * mainEnv = multiQueueEnv;
	multiQueueSystem();

	return 0;
}
