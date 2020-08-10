#ifndef DEF_H
#define DEF_H

#define BREAK_RANGE_START_SEC 1
#define BREAK_RANGE_END_SEC 600
#define DEBUG 1

#define debugPrintf(fmt, ...) \
	do {if (DEBUG) printf(fmt, __VA_ARGS__);} while (0)

// time in minutes
typedef double simTime;

#endif
