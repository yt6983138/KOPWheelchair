#include "Buzzer.h"

void Buzzer::Update() const
{
	auto now = micros();
	auto totalLength = this->OffMicroseconds + this->OnMicroseconds;
	auto nowCycleAt = now % totalLength;
	auto value = nowCycleAt > this->OffMicroseconds ? MapZeroToOneToUInt8(this->OnVolume) : MapZeroToOneToUInt8(this->OffVolume);
	analogWrite(this->Pin, value);
}
