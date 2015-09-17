/*
 * Template project for arduino due. This project
 * includes the standard functions from the Arduino
 * library.
 * @author Daniel Nordahl
 */

#include <stdio.h>
#include "Arduino.h"
#include <test_exampel.h>
#include "diag/Trace.h"
#include "blinkLED.h"
#include "unity.h"
#include "uart.h"

void testRunner(void);
void init_board(void);


/**
 * Test runnner function add new tests here.
 */

void testRunner(void)
{

	// Start unit test
	UnityBegin("");

	// Run ExampelCode tests
	Unity.TestFile = "..system/include/test/test_ExampelCode.h";
	RUN_TEST(test_ExampelCode);
	RUN_TEST(test_ExampelCode2);


	// End unit test
	UnityEnd();
}

/**
 * Sets up Arduino Due
 */

void init_board(){
	WDT->WDT_MR = WDT_MR_WDDIS; 		//Disable watchdog
	SystemInit();						// Initiate Due Card

	PIO_Configure(
		g_APinDescription[PINS_UART].pPort,
		g_APinDescription[PINS_UART].ulPinType,
		g_APinDescription[PINS_UART].ulPin,
		g_APinDescription[PINS_UART].ulPinConfiguration);
	digitalWrite(0, HIGH); // Enable pullup for RX0
}


int main(void) {


	init_board();
	testRunner();
	Serial.begin(9600);
    pinMode(A0,OUTPUT);
    pinMode(13,OUTPUT);
    analogWrite(A0,155);

    // Test function for blink L LED on Due connected to pin 13
    blinkLED();

	return 0;
}
