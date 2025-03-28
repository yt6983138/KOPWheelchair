// Utility.h

#ifndef _UTILITY_h
#define _UTILITY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define lengthof(array, element) sizeof(array) / sizeof(element)
#define EMPTY_CLOSURE []() {}

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef long long int64;
typedef unsigned long long uint64;

float FloatMap(float value, float srcLow, float srcHigh, float dstLow, float dstHigh);
void LogSerial(const char* format, ...);

#endif

