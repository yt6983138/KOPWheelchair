#include <PS2X_lib.h>
#include "Utility.h"
#include "KOPWheelchair.h"
#include "Motor.h"
#include "Light.h"
#include "Buzzer.h"

PS2X PS2;

Light SignalLight;
Buzzer SignalBuzzer;

Motor Left1; // must construct in setup
Motor Left2;
Motor Right1;
Motor Right2;

Motor* Motors[4] = { &Left1, &Left2, &Right1, &Right2 };

char ControlMode = 0;

static void SetAllMotor(float speed)
{
	for (auto motor : Motors)
		motor->SetSpeed(speed);
}
static void SetLeftMotors(float speed)
{
	Left1.SetSpeed(speed);
	Left2.SetSpeed(speed);
}
static void SetRightMotors(float speed)
{
	Right1.SetSpeed(speed);
	Right2.SetSpeed(speed);
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
	pinMode(7, INPUT);
	pinMode(8, OUTPUT);
	InitializeSerial();

	SignalLight.Pin = 7;
	SignalBuzzer.Pin = 8;
	
	SignalBuzzer.OnVolume = 0.51f;

	do
	{
		// setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
		auto error = PS2.config_gamepad(10, 12, 11, 13, false, true);
		if (error != 0)
		{
			LogDebug("PS2 error %d, refusing to initialize", error);
		}
		else
		{
			LogDebug("PS2 ok");
			break;
		}
	} while (1);

	SignalBuzzer.BeepFor(200);

	Left1 = Motor(5, true, COMMON_SPEED_MULTIPLER * LEFT_SPEED_MULTIPLER);
	Left2 = Motor(6, true, COMMON_SPEED_MULTIPLER * LEFT_SPEED_MULTIPLER);
	Right1 = Motor(3, false, COMMON_SPEED_MULTIPLER * RIGHT_SPEED_MULTIPLER);
	Right2 = Motor(4, false, COMMON_SPEED_MULTIPLER * RIGHT_SPEED_MULTIPLER);
}

// Add the main program code into the continuous loop() function
void loop()
{
	delay(10);

	if (PS2.read_gamepad(false, 0) != true)
		ControlMode = 0;

	SignalLight.Update();
	SignalBuzzer.Update();
	for (auto motor : Motors)
		motor->Update();

	auto controllerLeftX = -((float)PS2.Analog(PSS_LX) / 127.5f - 1.0f);
	auto controllerLeftY = -((float)PS2.Analog(PSS_LY) / 127.5f - 1.0f);
	auto controllerRightX = -((float)PS2.Analog(PSS_RX) / 127.5f - 1.0f);
	auto controllerRightY = -((float)PS2.Analog(PSS_RY) / 127.5f - 1.0f);

	LogDebug("%d %d %d %d", (int)(controllerLeftX * 100.0f), (int)(controllerLeftY * 100.0f), (int)(controllerRightX * 100.0f), (int)(controllerRightY * 100.0f));

	auto controllerPadUp = PS2.Button(PSB_PAD_UP);
	auto controllerPadDown = PS2.Button(PSB_PAD_DOWN);
	auto controllerPadLeft = PS2.Button(PSB_PAD_LEFT);
	auto controllerPadRight = PS2.Button(PSB_PAD_RIGHT);

	auto controllerL1 = PS2.Button(PSB_L1);
	auto controllerL2 = PS2.Button(PSB_L2);
	auto controllerR1 = PS2.Button(PSB_R1);
	auto controllerR2 = PS2.Button(PSB_R2);

	auto controllerTriangle = PS2.Button(PSB_TRIANGLE);
	auto controllerCircle = PS2.Button(PSB_CIRCLE);
	auto controllerCross = PS2.Button(PSB_CROSS);
	auto controllerSquare = PS2.Button(PSB_SQUARE);

	if (PS2.NewButtonState(PSB_SELECT) && PS2.Button(PSB_SELECT))
	{
		ControlMode++;
		if (ControlMode > 2) ControlMode = 0;
	}

	switch (ControlMode)
	{
	case 0:
		// default disabled mode
		SetAllMotor(0);
		SignalLight.OnMicroseconds = 1000000;
		SignalLight.OffMicroseconds = 0;
		break;
	case 1:
		SignalLight.OnMicroseconds = 200000;
		SignalLight.OffMicroseconds = 200000;
		if (controllerPadUp)
		{
			SetAllMotor(1);
		}
		else if (controllerPadDown)
		{
			SetAllMotor(-1);
		}
		else if (controllerPadLeft)
		{
			SetLeftMotors(-1);
			SetRightMotors(1);
		}
		else if (controllerPadRight)
		{
			SetLeftMotors(1);
			SetRightMotors(-1);
		}
		else
		{
			SetLeftMotors(controllerLeftY);
			SetRightMotors(controllerRightY);
		}
		break;
	case 2:
		SignalLight.OnMicroseconds = 400000;
		SignalLight.OffMicroseconds = 400000;

		// right: -, left: +, front: 0, back: +-1
		auto rotationDegreeScale = atan2(controllerLeftX, controllerLeftY) / 3.1415926f;
		auto rotationDegreeScaleAbs = fabs(rotationDegreeScale);
		auto power = sqrt(controllerLeftX * controllerLeftX + controllerLeftY * controllerLeftY);
		power = fmin(power, 1.0f);

		float leftWheelSpeed = 0;
		float rightWheelSpeed = 0;

		// left
		if (rotationDegreeScale >= 0.0f)
		{
			if (rotationDegreeScale <= 0.5f)
			{
				leftWheelSpeed = power * (1.0f - 4.0f * rotationDegreeScale);
				rightWheelSpeed = power;
			}
			// backward
			else
			{
				leftWheelSpeed = -power;
				rightWheelSpeed = power * (3.0f - 4.0f * rotationDegreeScale);
			}
		}
		// right
		else
		{
			if (rotationDegreeScale >= -0.5f)
			{
				leftWheelSpeed = power;
				rightWheelSpeed = power * (1.0f + 4.0f * rotationDegreeScale);
			}
			// backward
			else
			{
				leftWheelSpeed = power * (3.0f + 4.0f * rotationDegreeScale);
				rightWheelSpeed = -power;
			}
		}

		SetLeftMotors(leftWheelSpeed);
		SetRightMotors(rightWheelSpeed);
		break;
	default:
		break;
	}
}
