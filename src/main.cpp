#ifdef ARDUINO
/**
 * Main file used when developing the Calvin Arduino library within eclipse
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015
 *
 * Andreas Elvstam, Jesper Hansen, Peter Johansson, Daniel Nordahl, Carl Öström
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "test_exampel.h"
#include "Arduino.h"
#include <test_exampel.h>
#include "diag/Trace.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include "ArduinoJson.h"
#include "CalvinDone/CalvinMini.h"
#include "blinkLED.h"
#include "uart.h"
#include "CalvinDone/HandleSockets.h"

LiquidCrystal lcd(8,9,4,5,6,7);

void testRunner(void);
void init_board(void);

/**
 * Test runnner function add new tests here.
 */

void testRunner(void)
{


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

	// Enable movement sensor and speaker/led
	pinMode(22, INPUT);
	pinMode(31, OUTPUT);
	//digitalWrite(22, LOW);
	digitalWrite(31, LOW);

}

int main(void) {

	init_board();
	Serial.begin(115200);
	uint16_t port = 5002;

	byte macAdr[] = { 0xC0, 0xAA, 0xCB, 0xAB, 0x0E, 0x01 };
	IPAddress ip(192,168,0,60);
	String rtID = "Calvin6";

/*	byte macAdr[] = { 0xC0, 0xAA, 0xAB, 0xCB, 0x0E, 0x02 };
	IPAddress ip(192,168,0,20);
	String rtID = "Calvin2";

	byte macAdr[] = { 0xC0, 0xAA, 0xAB, 0xCB, 0x0E, 0x03 };
	IPAddress ip(192,168,0,30);
	String rtID = "Calvin3";
	/*
	byte macAdr[] = { 0xC0, 0xAA, 0xAB, 0xCB, 0x0E, 0x04 };
	IPAddress ip(192,168,0,40);
	String rtID = "Calvin4";

	byte macAdr[] = { 0xC0, 0xAA, 0xAB, 0xCB, 0x0E, 0x05 };
	IPAddress ip(192,168,0,50);
	String rtID = "Calvin5";
	byte macAdr[] = { 0xC0, 0xAA, 0xAB, 0xCB, 0x0E, 0x06 };
	IPAddress ip(192,168,0,60);
	String rtID = "Calvin6";
	*/
	CalvinMini mini(rtID, macAdr, ip, port);
	mini.loop();

}

#else

#include "gtest/gtest.h"

/**
 * This main function launch all the test that is written for the project
 * it only runs if the project is compiled and runne'd in mocking mode
 */

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
