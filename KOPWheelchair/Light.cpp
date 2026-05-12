#include "Light.h"

void Light::Update() const
{
	auto now = micros();
	auto totalLength = this->OffMicroseconds + this->OnMicroseconds;
	auto nowCycleAt = now % totalLength;
	auto value = nowCycleAt > this->OffMicroseconds ? MapZeroToOneToUInt8(this->OnBrightness) : MapZeroToOneToUInt8(this->OffBrightness);
	analogWrite(this->Pin, value);
}
