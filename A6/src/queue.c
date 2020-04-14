#include "queue.h"

node * createNode(event * e) {
	node * n = malloc(sizeof(node));
	n->eve = e;
	n->prev = NULL;
	n->next = NULL;
	return n;
}

void createFQueue(fifoQueue * fq) {
	fq->head = NULL;
	fq->tail = NULL;
	fq->length = 0;
}

void pushFQueue(fifoQueue * fq, node * n) {
	if (fq->length == 0) {
		fq->head = n;
		fq->tail = n;
		n->prev = NULL;
		n->next = NULL;
	} else {
		fq->tail->next = n;
		n->prev = fq->tail;
		fq->tail = n;
	}
	fq->length += 1;
}

node * popFQueue(fifoQueue * fq) {
	node * returnNode;
	if (fq->length == 0) {
		return NULL;
	} else if (fq->length == 1) {
		returnNode = fq->head;
		fq->head = NULL;
		fq->tail = NULL;
	} else {
		returnNode = fq->head;
		fq->head = returnNode->next;
		fq->head->prev = NULL;
	}
	fq->length -= 1;
	returnNode->prev = NULL;
	returnNode->next = NULL;
	return returnNode;
}
