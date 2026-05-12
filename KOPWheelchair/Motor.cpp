#include "Motor.h"
#include "Utility.h"

static float DefaultSpeedSmoother(float currentSpeed, float targetSpeed, uint64 elapsedMicros)
{
	return targetSpeed;
}

Motor::Motor(uint8 pwmPin, bool isReversed, float speedMultipler = 1, SpeedSmoothFunc* speedSmoother = NULL)
{
	IsReversed = isReversed;
	SpeedMultipler = speedMultipler;
	PWMController = Servo();

	PWMController.attach(pwmPin, 1000, 2000);
	SpeedSmoother = speedSmoother != NULL ? speedSmoother : DefaultSpeedSmoother;
}
Motor::Motor() = default;
Motor::~Motor()
{
	Stop();
}

void Motor::Update()
{
	auto now = micros();
	auto newSpeed = (*SpeedSmoother)(CurrentSpeed, TargetSpeed, now - LastPWMPulseMicros);
	CurrentSpeed = newSpeed;
	SetSpeedForce(newSpeed);
}
void Motor::SetSpeedForce(float speed)
{
	auto multipler = (IsReversed ? -1.0f : 1.0f) * SpeedMultipler;
	auto micros = (int16)(speed * multipler * 500.0f + 1500.0f);
	PWMController.writeMicroseconds(micros);
	LastPWMPulseMicros = micros;
}

/// <summary>
/// set speed
/// </summary>
/// <param name="speed">range from -1~1</param>
void Motor::SetSpeed(float speed)
{
	TargetSpeed = speed;
}
void Motor::Stop()
{
	SetSpeedForce(0);
}
