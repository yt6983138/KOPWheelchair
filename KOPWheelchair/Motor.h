// Motor.h

#include "Utility.h"
#include <Servo.h>

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
typedef float (*SpeedSmoothFunc)(float currentSpeed, float targetSpeed, uint64 elapsedMicros);

struct Motor
{
private:
	Servo PWMController;
	int16 LastPWMPulseMicros;
public:
	float SpeedMultipler;
	bool IsReversed;
	SpeedSmoothFunc* SpeedSmoother;

	Motor(uint8 pwmPin, bool isReversed, float speedMultipler = 1, SpeedSmoothFunc* speedSmoother = NULL);
	Motor();
	~Motor();

	void Update();
	void SetSpeed(float speed);
	void Stop();
};

#endif
