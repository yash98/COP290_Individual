#include "simulator.h"
#include "environment.h"
#include "event.h"

#include <malloc.h>
#include <stdlib.h>

environment * mainEnv;

simTime multiQueueSystem() {
	customer * lastCustomer = mainEnv->customers + mainEnv->customerCount - 1;

	customer * latestCustomer = mainEnv->customers;

	enum mode {customerMode, tellerMode} actionMode;
	simTime resumeTime = 0.0;

	while (lastCustomer->exitTime < 0) {
		if (actionMode == customerMode) {
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

			debugPrintf("%lf: customer %s entered queue %s\n", mainEnv->clock, latestCustomer->customerId, chosenQueue);
			
			latestCustomer++;
		} else {
			mainEnv->clock = resumeTime;
			for (int i = 0; i < mainEnv->numQueues; i++) {
				if (*(mainEnv->queueBusyTime+i) == resumeTime) {
					// End Queue
					simTime busyFor; 
					if ((mainEnv->endQueues+i)->length != 0) {
						// Currently all End Queue have 0 time utilized 
						event * eqEve = popFQueue(mainEnv->endQueues+i)->eve;
						busyFor = (eqEve->eventFunction) (eqEve->argVector);
						freeMemEvent(eqEve);

						if (busyFor > 0.0) {
							// Teller is taking a break
							continue;
						}
					}
					
					// Start Queue
					busyFor = 0.0;
					while (busyFor == 0.0) {
						event * sqEve = popFQueue(mainEnv->startQueues)->eve;
						busyFor = (sqEve->eventFunction) (sqEve->argVector);
						*(mainEnv->queueBusyTime+i) = mainEnv->clock + busyFor;
						freeMemEvent(sqEve);
					}
				}
			}
		}

		// Select next event for teller
		simTime resumeTime = 1.79769e+308; 
		for (int i = 0; i < mainEnv->numQueues; i++) {
			if (*(mainEnv->queueBusyTime+i) < resumeTime) {
				resumeTime = *(mainEnv->queueBusyTime+i);
			}
		}

		if (resumeTime > latestCustomer->enterTime) {
			actionMode = customerMode;
		} else {
			actionMode = tellerMode;
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
	
	debugPrintf("Initialization of multiqueue system finished\n")
	multiQueueSystem();

	return 0;
}
