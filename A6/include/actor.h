#ifndef ACTOR_H
#define ACTOR_H

#include "def.h"
#include "queue.h"

typedef struct tellerS
{
	int tellerId;
	time idleTime;
	time serviceTime;
	time breakDuration;
} teller;

typedef struct customerS
{
	int customerId;
	int lineIndex;
	time enterTime;
	time exitTime;
	time waitTime;
	time serviceTime;
} customer;

void createTeller(teller * t, int tId);
void createCustomer(customer * c, int cId);

#endif
