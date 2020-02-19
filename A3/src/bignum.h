#ifndef BIGNUM_H
#define BIGNUM_H

#include <float.h>

/**
unisgned int in light or or big endian dont care
float limited to 20 decimal points
**/
typedef struct vectorS {
	signed int* array;
	int size;
	int filled;
} vector;

/**
whole:
	convention least significant bits first in vector
sign:
	0 for 0 bignum
	-1 for negative +1 for positive
cutOff:
	if uint member of whole.array > cutOff carry
	can only be of form re (9)+
cutOffSize:
	digits in cutOff
**/
typedef struct bignumS {
	vector whole;
	float decimal;
	int sign;
	signed int cutOff;
	int cutOffSize;
} bignum;

int max(int a, int b);
int min(int a, int b);

void vInit(vector* v, int size);
void vDel(vector* v);
void pushback(vector* v, signed int x);
int pop(vector* v);

void bInit(bignum* b, char* s, signed int cut);
void bDel(bignum* b);
void str(bignum* b, char** c);

int isZero(bignum b);
int comp(bignum left, bignum right);
int compFloat(float left, float right);

void bignumCopy(bignum* dest, bignum source);

/**
c = a op b
**/
bignum addInternal(bignum a, bignum b, int signA, int signB);
bignum subInternal(bignum a, bignum b, int signA, int signB);

bignum add(bignum a, bignum b);
bignum sub(bignum a, bignum b);
bignum mul(bignum a, bignum b);
bignum div(bignum a, bignum b);

#endif
