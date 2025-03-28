#include "Utility.h"

float FloatMap(float value, float srcLow, float srcHigh, float dstLow, float dstHigh)
{
	float srcRange = srcHigh - srcLow;
	float dstRange = dstHigh - dstLow;

	float srcToDstRatio = dstRange / srcRange;

	float valueInSrcRange = value - srcLow;
	float valueInDstRange = valueInSrcRange * srcToDstRatio;

	float dst = valueInDstRange + dstLow;

	return dst;
}
void LogSerial(const char* format, ...)
{
	static char buffer[256];

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	Serial.println(buffer);
	memset(buffer, 0, sizeof(buffer));
}
