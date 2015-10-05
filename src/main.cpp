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
#include "CalvinInProgress/testJson.h"
#include "blinkLED.h"
#include "unity.h"
#include "uart.h"
#include "CalvinInProgress/calvinRuntime.h"

LiquidCrystal lcd(8,9,4,5,6,7);

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
  /*char json2[] = "{\"cmd\": \"JOIN_REQUEST\", \"serializers\": [\"json\"], \"id\": \"1c17dc88-63b0-48c2-8930-1df399b4c887\", \"sid\": \"MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb6\"}";
  char json3[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[1024,\"inside\"]}";
  StaticJsonBuffer<4096> jsonBuffer;
  JsonObject &msg = jsonBuffer.parseObject(json2);
  JsonObject &msg2 = jsonBuffer.parseObject(json3);
  String str = json.stringBuilderJsonObject(msg);
	str += json.stringBuilderJsonObject(msg2);
	Serial.println(str);
	char *test = json.jsonSerialize(str.c_str());
	Serial.println(test);*/
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
