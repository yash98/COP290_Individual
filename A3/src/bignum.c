#include "bignum.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int max(int a, int b) {
	if (a > b) return a;
	else return b;
}

int min(int a, int b) {
	if (a > b) return b;
	else return a;

}

void vInit(vector* v, int size) {
	v->array = (signed int*) malloc(size*sizeof(signed int));
	v->size = size;
	v->filled = 0;
}

void vDel(vector* v) {
	free(v->array);
}

void pushback(vector* v, signed int x) {
	if (v->filled == v->size) {
		v->array = (signed int*) realloc(v->array, 2*v->size*sizeof(signed int));
		v->size *= 2;
	}
	*(v->array+v->filled) = x;
	v->filled += 1;
}

int pop(vector* v) {
	v->filled -= 1;
	return *(v->array+v->filled+1);
}

void bInit(bignum* b, char* s, signed int cut) {
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
		b->cutOff = 9999;
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

	int storeSignA = a.sign;
	int storeSignB = b.sign;
	a.sign = signA;
	b.sign = signB;

	if (a.sign == b.sign) {
		bInit(&c, "0", 0);

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
			signed int p = 0;
			if (i < a.whole.filled) p += *(a.whole.array + i);
			if (i < b.whole.filled) p += *(b.whole.array + i);

			// carrying
			signed int lastCalc = *(c.whole.array + i - 1);
			if (lastCalc > c.cutOff) {
				p += (lastCalc / (c.cutOff + 1));
				*(c.whole.array + i - 1) = (lastCalc % (c.cutOff + 1));
			}

			pushback(&(c.whole), p);
		}

		// last carry
		signed int lastCalc = *(c.whole.array + i - 1);
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

bignum subInternal(bignum a, bignum b, int signA, int signB) {
	bignum c;

	int storeSignA = a.sign;
	int storeSignB = b.sign;
	a.sign = signA;
	b.sign = signB;

	if (a.sign == b.sign) {
		bInit(&c, "0", 0);
		if (a.sign == 0) return c;

		c.decimal = a.decimal - b.decimal;

		// carrying
		// floats less than 1.0 can't sum more equal to 2.0
		if (c.decimal <= 0.0) {
			*(c.whole.array) -= 1;
			c.decimal += 1.0;
		}

		int maxFilled = max(a.whole.filled, b.whole.filled);
		int minFilled = min(a.whole.filled, b.whole.filled);

		for (int i=1; i<maxFilled; i++) {
			pushback(&(c.whole), 0);
		}

		int signFixed = 0;

		for (int i=maxFilled-1; i >= 0; i--) {
			signed int* place = c.whole.array + i;

			if (i < a.whole.filled) *place += *(a.whole.array + i);
			// negative sign
			if (i < b.whole.filled) *place -= *(b.whole.array + i);

			if ((*place == 0) ) {
				if (!signFixed) {
					if (i != 0) {
						pop(&(c.whole));
					}
				}
			} else {
				if (!(signFixed)) {
					signFixed = 1;
					if (*place < 0) {
						c.sign = -1 * a.sign;
					} else {
						c.sign = a.sign;
					}
				}
			}

			if (i == maxFilled - 1) {
				if (*place < 0) {
					// don't need to minus as its already negative
					*place = c.cutOff + 1 + *place;
				}
			} else {
				if (*place < 0) {
					int nextPlace = i+1;
					while (nextPlace < c.whole.filled) {
						if (*(c.whole.array + nextPlace) != 0) {
							*(c.whole.array + nextPlace) -= 1;
							if ((*(c.whole.array + nextPlace) == 0) && (nextPlace == c.whole.filled)) {
								pop(&(c.whole));
							}
							nextPlace -= 1;
							break;	
						}
						nextPlace += 1;
					}
					while (nextPlace > i) {
						*(c.whole.array + nextPlace) = c.cutOff;
						nextPlace -= 1;
					}

					// don't need to minus as its already negative
					*(place) = c.cutOff + 1 + *place;
				}
			}

		}

		// if sub results in zero till end
		if (!signFixed) {
			c.sign = *(c.whole.array) * a.sign;
			*(c.whole.array) = 0;
		}

	} else {
		// revert sign and subtract
		c = addInternal(a, b, a.sign, -1 * b.sign);
	}

	a.sign = storeSignA;
	b.sign = storeSignB;

	return c;
}

int main() {
	bignum b1;
	bInit(&b1, "345078901234567890.0123456789", 0);
	char* p1;
	str(&b1, &p1);
	printf("%s\n", p1);

	bignum b2;
	bInit(&b2, "1234567890.99", 0);
	char* p2;
	str(&b2, &p2);
	printf("%s\n", p2);

	bignum c = subInternal(b1, b2, 1, 1);
	char* p3;
	str(&c, &p3);
	printf("%s\n", p3);

	bDel(&b1);
	free(p1);
	bDel(&b2);
	free(p2);
	bDel(&c);
	free(p3);
}
