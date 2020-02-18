#ifndef BIGNUM_H
#define BIGNUM_H

#include <float.h>

typedef struct vectorS {
	unsigned int* array;
	int size;
	int filled;
} vector;

typedef struct bignumS {
	vector whole;
	float decimal;
	int sign;
	unsigned int cutOff;
	int cutOffSize;
} bignum;

void vInit(vector* v, int size);
void vDel(vector* v);
void pushback(vector* v, int x);
int pop(vector* v);

void bInit(bignum* b, char* s, unsigned int cut);
void bDel(bignum* b);
void str(bignum* b, char** c);

int comp(bignum left, bignum right);

#endif
