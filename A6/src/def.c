#include "def.h"

#include <stdio.h>
#include <stdarg.h>

int debugPrintf(const char * fmt, ...) {
	if (DEBUG) {
		va_list args;
		va_start(args, fmt);
		int toReturn = vprintf(fmt, args);
		va_end(args);
		return toReturn;
	}
	return -1;
}

int doubleEquality(double a, double b) {
	double diff = a - b;
	return (diff < EPSILON) && (diff > -1.0 * EPSILON);
}
