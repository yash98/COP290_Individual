#ifndef QUEUE_H
#define QUEUE_H

#include "event.h"

/*
Heap based priority queue
Re-entry allowed and it gets sorted acc. to priority too
*/
typedef struct heapQueueS
{
	void * arr;
	int (* comp)(void *, void *);
	time doneTill;
} heapQueue;

/*
Queue Linked List based
Re-entry allowed (FIFO, no priority)
*/
typedef struct nodeS
{
	void * element;
	node * next;
} node;

typedef struct fifoQueueS
{
	node * head;
	int length;
} fifoQueue;

#endif
