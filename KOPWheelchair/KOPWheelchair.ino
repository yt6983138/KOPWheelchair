#include <PS2X_lib.h>
#include "Utility.h"
#include "KOPWheelchair.h"
#include "Motor.h"

PS2X PS2;

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
	Serial.begin(115200);

	// setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
	do
	{
		auto error = PS2.config_gamepad(13, 11, 12, 10, true, true);
		if (error != 0)
		{
			LogSerial("PS2 error %d, refusing to initialize", error);
			delay(100);
		}
		else
		{
			LogSerial("ps2 ok");
			break;
		}
	} while (1);

	Left1 = Motor(7, false, COMMON_SPEED_MULTIPLER);
	Left2 = Motor(6, false, COMMON_SPEED_MULTIPLER);
	Right1 = Motor(5, true, COMMON_SPEED_MULTIPLER);
	Right2 = Motor(4, true, COMMON_SPEED_MULTIPLER);
}

// Add the main program code into the continuous loop() function
void loop()
{
	/*PS2.read_gamepad();
	for (auto motor : Motors)
	{
		if (digitalRead(7) == HIGH)
		{
			motor->SetSpeed(0.5);
		}
		else motor->Stop();
	}

	delay(50);
	LogSerial("Joystick %d %d", PS2.Analog(PSS_LX), PS2.Analog(PSS_LY));
	return;*/

	delay(20);
	PS2.read_gamepad();

	auto controllerLeftX = -((float)PS2.Analog(PSS_LX) / 127.5f - 1.0f);
	auto controllerLeftY = -((float)PS2.Analog(PSS_LY) / 127.5f - 1.0f);
	auto controllerRightX = -((float)PS2.Analog(PSS_RX) / 127.5f - 1.0f);
	auto controllerRightY = -((float)PS2.Analog(PSS_RY) / 127.5f - 1.0f);

	//LogSerial("%d %d %d %d", (int)(controllerLeftX * 100.0f), (int)(controllerLeftY * 100.0f), (int)(controllerRightX * 100.0f), (int)(controllerRightY * 100.0f));

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
		break;
	case 1:
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
		auto rotationDegreeScale = atan2(controllerLeftX, controllerLeftY) / 3.1415926f;
		auto rotationDegreeScaleAbs = rotationDegreeScale < 0.0f ? 1.0f - rotationDegreeScale : rotationDegreeScale;

		auto leftSpeed = controllerLeftY * rotationDegreeScaleAbs;
		auto rightSpeed = controllerLeftY * (1.0f - rotationDegreeScaleAbs);
		break;
	default:
		break;
	}
}
