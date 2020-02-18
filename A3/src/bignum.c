#include "bignum.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void vInit(vector* v, int size) {
	v->array = (unsigned int*) malloc(size*sizeof(unsigned int));
	v->size = size;
	v->filled = 0;
}

void vDel(vector* v) {
	free(v->array);
}

void pushback(vector* v, int x) {
	if (v->filled == v->size) {
		v->array = (unsigned int*) realloc(v->array, 2*v->size*sizeof(unsigned int));
		v->size *= 2;
	}
	*(v->array+v->filled) = x;
	v->filled += 1;
}

int pop(vector* v) {
	v->filled -= 1;
	return *(v->array+v->filled+1);
}

void bInit(bignum* b, char* s, unsigned int cut) {
	// sign
	if(*s == '-') {
		b->sign = -1;
	} else {
		b->sign = 1;
	}
	
	char *sCopy;
	if (b->sign >= 0) sCopy = strdup(s);
	else sCopy = strdup(s+1);

	// decimal
	char* wholeStart;
	if (strchr(sCopy, '.')) {
		wholeStart = strtok(sCopy, ".");
		char* decimalStart = strtok(NULL, ".");

		decimalStart -= 1;
		*decimalStart = '.';
		b->decimal = strtof(decimalStart, NULL);
		*decimalStart = '\0';
		decimalStart += 1;
	} else {
		// handling string with no decimal
		wholeStart = sCopy;
		b->decimal = 0;
	}

	int notCopied = strlen(sCopy);

	// default cutOff
	if (cut == 0) {
		b->cutOff = 99999;
	} else {
		b->cutOff = cut;
	}

	char cutOffStr[11];
	sprintf(cutOffStr, "%d", b->cutOff);
	b->cutOffSize = strlen(cutOffStr);
	int wholeSize = strlen(wholeStart);
	int numCut = wholeSize / b->cutOffSize;
	if (wholeSize % b->cutOffSize != 0) numCut += 1;

	char wholeStr[b->cutOffSize+1];
	wholeStr[b->cutOffSize] = '\0';

	// construct whole
	vInit(&(b->whole), numCut);
	while(notCopied > 0) {
		int i;
		if (notCopied > b->cutOffSize) {
			i = 0;
			strncpy(wholeStr, sCopy+notCopied-b->cutOffSize, sizeof(char)*b->cutOffSize);
			notCopied -= b->cutOffSize;
		} else {
			i = b->cutOffSize-notCopied;
			strncpy(&wholeStr[i], sCopy, sizeof(char)*notCopied);
			notCopied = 0;
		}

		pushback(&(b->whole), atoi(&wholeStr[i]));
	}

	if (isZero(*b)) b->sign = 0;
	free(sCopy);
}

void bDel(bignum* b) {
	vDel(&(b->whole));
}

void str(bignum* b, char** c) {
	int strSize = (b->whole).filled * b->cutOffSize + 20 + 1;

	*c = malloc(sizeof(char)*strSize);
	char* w = *c;
	char wholeStr[b->cutOffSize+1];
	wholeStr[b->cutOffSize] = '\0';

	// write whole in
	for (int i = b->whole.filled-1; i >= 0; i--) {
		int val = *((b->whole).array + i);
		sprintf(wholeStr, "%d", val);
		int length = strlen(wholeStr);
		
		if (i != b->whole.filled-1) {
			for (int j = 0; j < b->cutOffSize; j++) {
				int leftOut = b->cutOffSize - length;
				if (j < leftOut) {
					*w = '0';
				} else {
					*w = wholeStr[j - leftOut];
				}
				w += 1;
			}
		} else {
			strcpy(w, wholeStr);
			w += strlen(wholeStr);
		}
	}

	// write decimal in
	// float limited to 20 decimal points
	if (b->decimal != 0) {
		char decimalStr[23];
		gcvt(b->decimal, 22, decimalStr);
		decimalStr[22] = '\0';
		char* deciRead = &decimalStr[1];
		while (*deciRead != '\0') {
			*w = *(deciRead++);
			w += 1;
		}
	}
	*w = '\0';
}

int isZero(bignum b) {
	if (b.decimal != 0) return 0;

	for (int i=0; i < b.whole.filled; i++) {
		if (*(b.whole.array+i) != 0) return 0;
	}
	return 1;
}

int compFloat(float left, float right) {
	if (left == right) return 0;
	else if (left > right) return 1;
	return -1;
}

int comp(bignum left, bignum right) {
	int isLeftZero = isZero(left);
	int isRightZero = isZero(right);

	if (isLeftZero && isRightZero) return 0;
	if (isLeftZero) return -1 * right.sign;
	if (isRightZero) return left.sign;

	// none is zero
	if (left.sign != right.sign) {
		// left neg its less left positive its greater
		return left.sign;
	} else {
		if (left.whole.filled != right.whole.filled) {
			if (left.whole.filled > right.whole.filled) {
				return left.sign;
			} else {
				return -1 * left.sign;
			}
		} else {
			// same filled
			if (left.sign > 0) {
				for (int i=left.whole.filled-1; i>=0; i++) {
					if (*(left.whole.array+i) > *(right.whole.array+i)) return 1;
					else return -1;
				}

				// same till now (all whole passed)
				return compFloat(left.decimal, right.decimal);
			} else {
				for (int i=left.whole.filled-1; i>=0; i++) {
					// change return signs
					if (*(left.whole.array+i) > *(right.whole.array+i)) return -1;
					else return 1;
				}

				// same till now (all whole passed)
				// revert sign as float doesn't represent it
				return -1 * compFloat(left.decimal, right.decimal);
			}
		}
	}

	// error if returned -2
	return -2;
}

void bignumCopy(bignum* dest, bignum source) {
	dest->decimal = source.decimal;
	dest->sign = source.sign;
	dest->cutOff = source.cutOff;
	dest->cutOffSize = source.cutOffSize;

	// copt whole
	vInit(&(dest->whole), source.whole.size);
	dest->whole.filled = source.whole.filled;
	for (int i=0; i<source.whole.filled; i++) {
		*(dest->whole.array + i) = *(source.whole.array + i);
	}
}

bignum addInternal(bignum a, bignum b, int signA, int signB) {
	bignum c;
	bInit(&c, "0", 0);

	int storeSignA = a.sign;
	int storeSignB = b.sign;
	a.sign = signA;
	b.sign = signB;

	if (a.sign == b.sign) {
		if (a.sign == 0) return c;

		c.sign = a.sign;
		c.decimal = a.decimal + b.decimal;
		*(c.whole.array) = *(a.whole.array) + *(b.whole.array);

		// carrying
		// floats less than 1.0 can't sum more equal to 2.0
		if (c.decimal >= 1.0) {
			*(c.whole.array) += 1;
			c.decimal -= 1.0;
		}

		int maxFilled = max(a.whole.filled, b.whole.filled);
		int minFilled = min(a.whole.filled, b.whole.filled);

		int i;
		for (i=1; i < maxFilled; i++) {
			unsigned int p = 0;
			if (i < a.whole.filled) p += *(a.whole.array + i);
			if (i < b.whole.filled) p += *(b.whole.array + i);

			// carrying
			unsigned int lastCalc = *(c.whole.array + i - 1);
			if (lastCalc > c.cutOff) {
				p += (lastCalc / (c.cutOff + 1));
				*(c.whole.array + i - 1) = (lastCalc % (c.cutOff + 1));
			}

			pushback(&(c.whole), p);
		}

		// last carry
		unsigned int lastCalc = *(c.whole.array + i - 1);
		if (lastCalc > c.cutOff) {
			pushback(&(c.whole), (lastCalc / (c.cutOff + 1)));
			*(c.whole.array + i - 1) = (lastCalc % (c.cutOff + 1));
		}
	} else {
		// revert sign and subtract
		c = subInternal(a, b, a.sign, -1 * b.sign);
	}

	a.sign = storeSignA;
	b.sign = storeSignB;

	return c;
}

int main() {
	vector v1;
	vInit(&v1, 10);
	bignum b1;
	bignum b2;
	bInit(&b1, "345078901234567890.0123456789", 0);
	bInit(&b2, "0", 0);
	char* p1;
	str(&b1, &p1);
	printf("%s\n", p1);
	char* p2;
	str(&b2, &p2);
	printf("%s\n", p2);
	vDel(&v1);
	bDel(&b1);
	free(p1);
	bDel(&b2);
	free(p2);
}
