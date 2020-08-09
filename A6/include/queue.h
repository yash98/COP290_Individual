#ifndef QUEUE_H
#define QUEUE_H

#include "event.h"

/*
Heap based priority queue
Re-entry allowed and it gets sorted acc. to priority too
Primary purpose is timed events sorting
*/
// Not needed hence not implemented right now
typedef struct heapQueueS
{
	timedEvent * tEve;
	int (* comp)(void *, void *);
	simTime doneTill;
} heapQueue;

/*
Queue Linked List based
Re-entry allowed (FIFO, no priority)
Primary purpose maintaining order amongst non timed events
*/
typedef struct nodeS
{
	event * eve;
	struct nodeS * prev;
	struct nodeS * next;
} node;

node * createNode(event * e);

typedef struct fifoQueueS
{
	node * head;
	node * tail;
	int length;
} fifoQueue;

void createFQueue(fifoQueue * fq);
void pushFQueue(fifoQueue * fq, node * n);
node * popFQueue(fifoQueue * fq);

#endif
