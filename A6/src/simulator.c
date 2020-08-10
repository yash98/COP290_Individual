#include "simulator.h"
#include "environment.h"
#include "event.h"
#include "def.h"

#include <malloc.h>
#include <stdlib.h>

environment * mainEnv;

simTime multiQueueSystem() {
	customer * lastCustomer = mainEnv->customers + mainEnv->customerCount - 1;

	customer * latestCustomer = mainEnv->customers;

	simTime resumeTime = 0.0;
	int takingMoreCustomer = 1;
	int customersPresent = 0;
	enum mode {customerMode, tellerMode} actionMode = tellerMode;

	while ((mainEnv->clock < mainEnv->totalTime) || customersPresent) {
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
			sprintf(*createdArgV, "%d", chosenQueue);
			sprintf(*(createdArgV+1), "%d", latestCustomer->customerId);

			pushFQueue(mainEnv->startQueues+chosenQueue, createNode(createEvent(&serveCustomer, 2, createdArgV)));
			latestCustomer->lineIndex = chosenQueue;

			debugPrintf("%lf: customer %d entered queue %d\n", mainEnv->clock, latestCustomer->customerId, chosenQueue);

			customersPresent = 1;
			
			if (latestCustomer == lastCustomer) {
				takingMoreCustomer = 0;
			} else {
				latestCustomer++;
			}
		} else {
			mainEnv->clock = resumeTime;
			for (int i = 0; i < mainEnv->numQueues; i++) {
				if (doubleEquality(*(mainEnv->queueBusyTime+i), resumeTime)) {
					// End Queue
					simTime busyFor; 
					// Currently all End Queue have 0 time utilized 
					node * poppedEqNode = popFQueue(mainEnv->endQueues+i);
					event * eqEve = poppedEqNode->eve;
					busyFor = (eqEve->eventFunction) (eqEve->argVector);
					freeMemNode(poppedEqNode);

					if (busyFor > 0.0) {
						// Teller is taking a break
						*(mainEnv->queueBusyTime+i) = mainEnv->clock + busyFor;
						continue;
					}
					
					// Start Queue
					busyFor = 0.0;
					node * poppedSqNode = popFQueue(mainEnv->startQueues+i);
					event * sqEve = poppedSqNode->eve;
					busyFor = (sqEve->eventFunction) (sqEve->argVector);
					*(mainEnv->queueBusyTime+i) = mainEnv->clock + busyFor;
					freeMemNode(poppedSqNode);
				}
			}

			// Select next event for teller
			resumeTime = 1.79769e+308;
			customersPresent = 0;
			for (int i = 0; i < mainEnv->numQueues; i++) {
				if (*(mainEnv->queueBusyTime+i) < resumeTime) {
					resumeTime = *(mainEnv->queueBusyTime+i);
				}
				if ((mainEnv->startQueues+i)->length > 0) {
					customersPresent = 1;
				}
			}
		}

		if ((takingMoreCustomer) && (resumeTime > latestCustomer->enterTime)) {
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
	simTime avgTellerServiceTime = atof(argv[4]);

	environment * multiQueueEnv = (environment *) malloc(sizeof(environment));
	createEnv(multiQueueEnv, numTellers, avgTellerServiceTime, numCustomers, totalTime);
	mainEnv = multiQueueEnv;
	
	debugPrintf("Initialized of multiqueue system\n");
	multiQueueSystem();

	return 0;
}
