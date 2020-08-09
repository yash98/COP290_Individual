#include "actor.h"

#include <stdlib.h>

void createTeller(teller * t, int tId) {
	t->tellerId = tId,
	t->idleTime = 0.0;
	t->serviceTime = 0.0;
	t->breakDuration =  ((rand()/((time) RAND_MAX))*(BREAK_RANGE_END_SEC - BREAK_RANGE_START_SEC) + BREAK_RANGE_START_SEC) / 60.0;
}

void createCustomer(customer * c, int cId, time enterTime) {
	c->customerId = cId;
	c->lineIndex = -1;
	c->enterTime = enterTime;
	c->exitTime = -1.0;
	c->waitTime = 0.0;
	c->serviceTime = 0.0;
}
