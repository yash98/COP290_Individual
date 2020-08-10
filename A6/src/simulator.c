#include "simulator.h"
#include "environment.h"
#include "event.h"
#include "def.h"

#include <malloc.h>
#include <stdlib.h>
#include <math.h>

environment * mainEnv;

simTime multiQueueSystem() {
	customer * lastCustomer = mainEnv->customers + mainEnv->customerCount - 1;

	customer * latestCustomer = mainEnv->customers;

	simTime resumeTime = 0.0;
	int takingMoreCustomer = 1;
	int customersPresent = 0;
	simTime timeRequiredToServe = -1.0;
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
					// Only working tellers reach this
					if (*(mainEnv->queueBusyTime+i) > timeRequiredToServe) timeRequiredToServe = *(mainEnv->queueBusyTime+i);
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

	for (int i = 0; i < mainEnv->numQueues; i++) {
		teller * currentTeller = mainEnv->tellers + i;
		simTime diff = (currentTeller->idleTime + currentTeller->serviceTime) - timeRequiredToServe;

		if (timeRequiredToServe > 0.0) {
			currentTeller->idleTime -= diff;
		} else if (timeRequiredToServe < 0.0) {
			currentTeller->idleTime += diff;
		}
	}

	return timeRequiredToServe;
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
	simTime timeRequired = multiQueueSystem();

	printf("\nMULTIPLE QUEUE\n\n");
	printf("Number of Customers served: %d\n", numCustomers);
	printf("Total time required to serve customers: %lf minutes\n", timeRequired);
	printf("Number of Tellers: %d\n", numTellers);
	printf("Each teller has their own queue\n");

	simTime meanTimeSpentCustomer = 0.0;
	for (int i = 0; i < numCustomers; i++) {
		meanTimeSpentCustomer += (mainEnv->customers+i)->exitTime - (mainEnv->customers+i)->enterTime;
	}
	meanTimeSpentCustomer /= numCustomers;

	simTime stddevTimeSpentCustomers = 0.0;
	for (int i = 0; i < numCustomers; i++) {
		stddevTimeSpentCustomers += pow(((mainEnv->customers+i)->exitTime - (mainEnv->customers+i)->enterTime) - meanTimeSpentCustomer, 2);
	}
	stddevTimeSpentCustomers /= numCustomers;
	stddevTimeSpentCustomers = sqrt(stddevTimeSpentCustomers);

	printf("Mean time spent by customer in the bank: %lf minutes\n", meanTimeSpentCustomer);
	printf("Standard deviation of time spent by customer in the bank: %lf minutes\n", stddevTimeSpentCustomers);

	simTime maxWaitTime = -1.0;
	for (int i = 0; i < numCustomers; i++) {
		if ((mainEnv->customers+i)->waitTime > maxWaitTime) {
			maxWaitTime =(mainEnv->customers+i)->waitTime;
		}
	}

	printf("Maximum time a customer had to wait to see a teller: %lf minutes\n", maxWaitTime);

	simTime tellersServiceTime;
	simTime tellerIdleTime;

	for (int i = 0; i < numTellers; i++) {
		tellersServiceTime += (mainEnv->tellers+i)->serviceTime;
		tellerIdleTime += (mainEnv->tellers+i)->idleTime;
	}

	printf("Total service time for tellers: %lf minutes\n", tellersServiceTime);
	printf("Total idle time for tellers: %lf minutes\n", tellerIdleTime);

	return 0;
}
