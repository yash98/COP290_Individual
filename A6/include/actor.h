#ifndef ACTOR_H
#define ACTOR_H

#include "def.h"
#include "queue.h"

typedef struct tellerS
{
	int tellerId;
	simTime idleTime;
	simTime serviceTime;
	simTime breakDuration;
} teller;

typedef struct customerS
{
	int customerId;
	int lineIndex;
	simTime enterTime;
	simTime exitTime;
	simTime waitTime;
	simTime serviceTime;
} customer;

void createTeller(teller * t, int tId);
void createCustomer(customer * c, int cId, simTime enterTime);

#endif
