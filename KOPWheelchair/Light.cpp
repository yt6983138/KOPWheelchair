#include "Light.h"

void Light::Update() const 
{
	auto now = micros();
	auto totalLength = this->OffMicroseconds + this->OnMicroseconds;
	auto nowCycleAt = now % totalLength;
	auto value = nowCycleAt > this->OnMicroseconds ? FloatToUint8(this->OnBrightness) : FloatToUint8(this->OffBrightness);
	//Serial.println(value);
	analogWrite(this->Pin, value);
}
uint8 Light::FloatToUint8(float f) const
{
	return (uint8)(f * 255.0f);
}
