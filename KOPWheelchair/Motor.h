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

struct Motor
{
private:
	Servo PWMController;
	int16 LastPWMPulseMicros;
public:
	float SpeedMultipler;
	bool IsReversed;

	Motor(uint8 pwmPin, bool isReversed, float speedMultipler = 1);
	Motor();
	~Motor();

	void SetSpeed(float speed);
	void Stop();
};

#endif
