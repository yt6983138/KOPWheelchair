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

// The setup() function runs once each time the micro-controller starts
void setup()
{
	pinMode(7, INPUT);
	Serial.begin(115200);

	auto error = PS2.config_gamepad(13, 11, 10, 12, true, true);
	if (error != 0)
	{
		LogSerial("PS2 error %d, refusing to initialize", error);
		delay(~0);
	}

	Left1 = Motor(3, false, COMMON_SPEED_MULTIPLER);
	Left2 = Motor(4, true, COMMON_SPEED_MULTIPLER);
	Right1 = Motor(5, false, COMMON_SPEED_MULTIPLER);
	Right2 = Motor(6, true, COMMON_SPEED_MULTIPLER);
}

// Add the main program code into the continuous loop() function
void loop()
{
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
}
