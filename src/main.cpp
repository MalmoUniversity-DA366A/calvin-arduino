/*
 * Template project for arduino due. This project
 * includes the standard functions from the Arduino
 * library.
 * @author Daniel Nordahl
 */

#include <stdio.h>
#include "Arduino.h"
#include "diag/Trace.h"
#include "../system/include/blinkLED/blinkLED.h"
#include "../system/include/unity/unity.h"
#include "../system/include/test/test_hej.h"


int main() {

	UnityBegin("..system/include/test/test_hej.h");

	RUN_TEST(test_hej);

	UnityEnd();

	WDT->WDT_MR = WDT_MR_WDDIS; 		//Disable watchdog
	SystemInit();						//Set up cpu clock
	PIO_Configure(						// Enable UART
	    g_APinDescription[PINS_UART].pPort,
	    g_APinDescription[PINS_UART].ulPinType,
	    g_APinDescription[PINS_UART].ulPin,
		g_APinDescription[PINS_UART].ulPinConfiguration);
	digitalWrite(0, HIGH); // Enable pullup for RX0

	Serial.begin(9600);
    pinMode(A0,OUTPUT);
    pinMode(13,OUTPUT);
    analogWrite(A0,155);

    blinkLED();

}
