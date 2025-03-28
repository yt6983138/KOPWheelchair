#include <Servo.h>
#include "Utility.h"
#include "KOPWheelchair.h"
#include "Motor.h"

Motor Left1; // must construct in setup
Motor Left2;
Motor Right1;
Motor Right2;

Motor* Motors[4] = { &Left1, &Left2, &Right1, &Right2 };

// The setup() function runs once each time the micro-controller starts
void setup()
{
	pinMode(7, INPUT);
	//Serial.begin(9600);
	Left1 = Motor(TIMER2_PIN3, false, COMMON_SPEED_MULTIPLER);
	Left2 = Motor(TIMER1_PIN9, true, COMMON_SPEED_MULTIPLER);
	Right1 = Motor(TIMER1_PIN10, false, COMMON_SPEED_MULTIPLER);
	Right2 = Motor(TIMER2_PIN11, true, COMMON_SPEED_MULTIPLER);
}

// Add the main program code into the continuous loop() function
void loop()
{
	if (digitalRead(7) == HIGH)
	{
		Left1.SetSpeed(0.5);
	}
	else Left1.Stop();
	delay(50);
}
