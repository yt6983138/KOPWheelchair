#pragma once

#include "utility.h"
struct Light
{
private:
	uint8 FloatToUint8(float f) const;
public:
	uint8 Pin;
	float OnBrightness = 1; // 0 ~ 1
	float OffBrightness = 0; // 0 ~ 1
	uint32 OnMicroseconds = 0;
	uint32 OffMicroseconds = 1;

	void Update() const;
};
