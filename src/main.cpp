#ifdef ARDUINO
/*
 * Template project for arduino due. This project
 * includes the standard functions from the Arduino
 * library.
 * @author Daniel Nordahl
 */
#include "test_exampel.h"
#include "Arduino.h"
#include <test_exampel.h>
#include "diag/Trace.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include "ArduinoJson.h"
#include "testJson.h"
#include "blinkLED.h"
#include "unity.h"
#include "uart.h"
#include "calvinRuntime.h"
//#include "telnet.h"

LiquidCrystal lcd(8,9,4,5,6,7);

#define MAX_LENGTH 255
#define TERMINATOR 0x0A // $0A in Terminal.exe

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
	Serial.begin(115200);
	calvinRuntime json;
	json.setupConnection();

	// Start test
	/*testJson test;
	StaticJsonBuffer<200> jsonBuffer;*/
	// Test 1
	/*JsonObject &root = jsonBuffer.createObject();
	test.test(root);
	root.printTo(Serial);*/
	// Test 2
	/*char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[1024,\"inside\"]}";
	JsonObject &root = jsonBuffer.parseObject(json);
	String str = test.buildStringFromJsonObject(root);
	Serial.println(str);*/
	// End test
}
#else

#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
