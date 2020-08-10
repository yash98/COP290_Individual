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
	enum mode {customerMode, tellerMode} actionMode = tellerMode;

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
			sprintf(*createdArgV, "%d", chosenQueue);
			sprintf(*(createdArgV+1), "%d", latestCustomer->customerId);

			pushFQueue(mainEnv->startQueues+chosenQueue, createNode(createEvent(&serveCustomer, 2, createdArgV)));
			latestCustomer->lineIndex = chosenQueue;

			debugPrintf("%lf: customer %d entered queue %d\n", mainEnv->clock, latestCustomer->customerId, chosenQueue);
			
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
					if ((mainEnv->endQueues+i)->length != 0) {
						// Currently all End Queue have 0 time utilized 
						node * poppedNode = popFQueue(mainEnv->endQueues+i);
						event * eqEve = poppedNode->eve;
						busyFor = (eqEve->eventFunction) (eqEve->argVector);
						freeMemNode(poppedNode);

						if (busyFor > 0.0) {
							// Teller is taking a break
							*(mainEnv->queueBusyTime+i) = mainEnv->clock + busyFor;
							printf("Queue %d busy till %lf\n", i, *(mainEnv->queueBusyTime+i));
							continue;
						}
					}
					
					// Start Queue
					busyFor = 0.0;
					while (doubleEquality(busyFor, 0.0)) {
						node * poppedNode = popFQueue(mainEnv->startQueues+i);
						event * sqEve = poppedNode->eve;
						busyFor = (sqEve->eventFunction) (sqEve->argVector);
						*(mainEnv->queueBusyTime+i) = mainEnv->clock + busyFor;
						printf("Queue %d busy till %lf\n", i, *(mainEnv->queueBusyTime+i));
						freeMemNode(poppedNode);
					}
				}
			}

			// Select next event for teller
			resumeTime = 1.79769e+308; 
			for (int i = 0; i < mainEnv->numQueues; i++) {
				if (*(mainEnv->queueBusyTime+i) < resumeTime) {
					resumeTime = *(mainEnv->queueBusyTime+i);
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
	simTime avgTellerServeTime = atof(argv[3]);

	environment * multiQueueEnv = (environment *) malloc(sizeof(environment));
	createEnv(multiQueueEnv, numTellers, avgTellerServeTime, numCustomers, totalTime);
	mainEnv = multiQueueEnv;
	
	debugPrintf("Initialized of multiqueue system\n");
	multiQueueSystem();

	return 0;
}
