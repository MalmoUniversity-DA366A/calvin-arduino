/*
 * ledActor.cpp
 *
 *  Created on: 29 okt. 2015
 *      Author: Andreas
 */

#include "Arduino.h"
#include "ledActor.h"

#define LED_RED		22
#define LED_YELLOW	24
#define LED_GREEN	26

uint32_t controlLed(uint32_t id)
{
	switch(id)
	{
		case(0):
			digitalWrite(LED_RED, LOW);
			digitalWrite(LED_YELLOW, LOW);
			digitalWrite(LED_GREEN, LOW);
			return id;
		case(1):
			digitalWrite(LED_RED, HIGH);
			return id;
		case(2):
			digitalWrite(LED_YELLOW, HIGH);
			return id;
		case(3):
			digitalWrite(LED_GREEN, HIGH);
			return id;
		default:
			return 255;
	}
}

void setupLedOut()
{
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_YELLOW, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
}

void testLedLoop()
{
	setupLedOut();
	while(1)
	{
		for(uint8_t i = 0; i<4; i++)
		{
			controlLed(i);
			delay(500);
		}
	}
}
