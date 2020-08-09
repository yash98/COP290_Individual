#include "simulator.h"
#include "environment.h"

#include <malloc.h>
#include <stdlib.h>

environment * mainEnv;

time multiQueueSystem() {

	customer * lastCustomer = mainEnv->customers + mainEnv->customerCount - 1;

	while (lastCustomer->exitTime < 0) {
		// Start Queue

		// End Queue
	}
}

int main(int argc, char * argv[]) {
	// Cmd Args
	int numCustomers = atoi(argv[1]);
	int numTellers = atoi(argv[2]);
	time totalTime = atof(argv[3]);
	time avgTellerServeTime = atof(argv[3]);

	environment * multiQueueEnv = (environment *) malloc(sizeof(environment));
	createEnv(multiQueueEnv, numTellers, avgTellerServeTime, numCustomers, totalTime);
	environment * mainEnv = multiQueueEnv;
	multiQueueSystem();

	return 0;
}
