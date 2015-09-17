/*
 * Template project for arduino due. This project
 * includes the standard functions from the Arduino
 * library.
 * @author Daniel Nordahl
 */

#include "test_exampel.h"
#include "Arduino.h"
#include "diag/Trace.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include "ArduinoJson.h"
#include "testJson.h"
#include "blinkLED.h"
#include "unity.h"
#include "uart.h"

LiquidCrystal lcd(8,9,4,5,6,7);
EthernetClient client;
// Enter a MAC address for your controller below.
byte mac[] = {
		  0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93
		};

#define MAX_LENGTH 255
#define TERMINATOR 0x0A // $0A in Terminal.exe

void testRunner(void);
void init_board(void);
void testJsonCode(void);
void initEthernet(void);
void printMyIp(void);

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

/**
 * Test Json serialize and unserialize
 * functions from user terminal input
 */
void testJsonCode(void)
{
  char temp[MAX_LENGTH+1]; // Make room for NULL terminator
  while(true)
  {
      String str = "";

      int size = Serial.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
      temp[size-1] = '\0';
      Serial.println(temp); // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
      if(size)              // or      {"sensor":"gps","time":"flies"}
      {
          // Json to String
          //str = jsonUnserialize(temp);
          //printJson(str);     // Prints: {"sensor":"gps","time":"flies"}

          // String to Json
          str = jsonSerialize(temp);
          Serial.println(str);  // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
      }
    }
}

 /**
 * Sets up Ethernet connection with DHCP
 */
void initEthernet()
{
	if (Ethernet.begin(mac) == 0)
	{
		Serial.println("Failed to configure Ethernet using DHCP");
	}
}

/**
 * Prints the IP-addres assigned to the Ethernet shield.
 */
void printMyIp()
{
	Serial.println();
	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++)
	{
		// print the value of each byte of the IP address:
    	Serial.print(Ethernet.localIP()[thisByte], DEC);
    	Serial.print(".");
    }
    Serial.println();
}

int main() {

	init_board();
	initEthernet();
	testRunner();
	Serial.begin(115200);
	testJsonCode();

	/*lcd.begin(16,2);
  pinMode(A0,OUTPUT);
  pinMode(13,OUTPUT);

  analogWrite(A0,155);
  lcd.write("Hello World");
  lcd.setCursor(0,1);
  lcd.write("Calvin mini");

  while(1){				//Run loop
      digitalWrite(13,HIGH);
    	delayMicroseconds(1000000);
    	digitalWrite(13,LOW);
    	delay(1000);
    	Serial.write(55);
    }*/


}
