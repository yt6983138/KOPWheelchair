#include "Motor.h"
#include "Utility.h"

Motor::Motor(uint8 pwmPin, bool isReversed, float speedMultipler = 1)
{
	IsReversed = isReversed;
	SpeedMultipler = speedMultipler;
	PWMController = Servo();

	PWMController.attach(pwmPin, 1000, 2000);
}
Motor::Motor() = default;
Motor::~Motor()
{
	Stop();
}

/// <summary>
/// set speed
/// </summary>
/// <param name="speed">range from -1~1</param>
void Motor::SetSpeed(float speed)
{
	auto multipler = ((IsReversed << 1) - 1) * SpeedMultipler;
	auto micros = (int16)(speed * multipler * 500.0f + 1500.0f);
	PWMController.writeMicroseconds(micros);
	LastPWMPulseMicros = micros;
}
void Motor::Stop()
{
	SetSpeed(0);
}
