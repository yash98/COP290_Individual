#include "actor.h"

#include <stdlib.h>

void createTeller(teller * t, int tId) {
	t->tellerId = tId,
	t->idleTime = 0.0;
	t->serviceTime = 0.0;
	t->breakDuration =  ((time) (rand()%BREAK_RANGE_END_SEC + 1))/60.0;
}

void createCustomer(customer * c, int cId) {
	c->customerId = cId;
	c->lineIndex = -1;
	c->enterTime = -1.0;
	c->exitTime = -1.0;
	c->waitTime = 0.0;
	c->serviceTime = 0.0;
}
