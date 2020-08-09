#include "simulator.h"
#include "environment.h"

#include <malloc.h>
#include <stdlib.h>

environment * mainEnv;

time multiQueueSystem() {
	return 0;
}

int main(int argc, char * argv[]) {
	int numCustomers = atoi(argv[1]);
	int numTellers = atoi(argv[2]);
	time totalTime = atof(argv[3]);
	time avgTellerServeTime = atof(argv[3]);
	environment * multiQueueEnv = (environment *) malloc(sizeof(environment));
	createEnv(multiQueueEnv, numTellers, avgTellerServeTime, numCustomers);
	environment * mainEnv = multiQueueEnv;
	return 0;
}
