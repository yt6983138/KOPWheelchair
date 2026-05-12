#pragma once

#include "utility.h"

struct Buzzer
{
public:
	uint8 Pin;
	float OnVolume = 1; // 0 ~ 1
	float OffVolume = 0; // 0 ~ 1
	uint32 OnMicroseconds = 0;
	uint32 OffMicroseconds = 1;

	void Update() const;
};
