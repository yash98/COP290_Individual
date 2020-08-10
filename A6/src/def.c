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
