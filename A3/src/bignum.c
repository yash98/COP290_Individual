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
	char* iter = s;

	// sign
	if(*iter == '-') {
		b->sign = -1;
		iter += 1;
	} else {
		b->sign = 1;
	}

	char *sCopy = strdup(s);
	char* wholeStart = strtok(sCopy, ".");
	char* decimalStart = strtok(NULL, ".");

	// decimal
	decimalStart -= 1;
	*decimalStart = '.';
	b->decimal = strtof(decimalStart, NULL);
	*decimalStart = '\0';
	decimalStart += 1;

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

int main() {
	vector v1;
	vInit(&v1, 10);
	bignum b1;
	bInit(&b1, "345078901234567890.0123456789", 0);
	char* p1;
	str(&b1, &p1);
	printf("%s\n", p1);
	vDel(&v1);
	bDel(&b1);
	free(p1);
}
