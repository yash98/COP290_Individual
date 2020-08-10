#ifndef DEF_H
#define DEF_H

#define EPSILON 0.00001

#define BREAK_RANGE_START_SEC 1
#define BREAK_RANGE_END_SEC 600

#define DEBUG 1

int debugPrintf(const char * fmt, ...);

// time in minutes
typedef double simTime;
int doubleEquality(double a, double b);

#endif
