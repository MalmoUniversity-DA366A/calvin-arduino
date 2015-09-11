/*
 * Template project for arduino due. This project
 * includes the standard functions from the Arduino
 * library.
 * @author Daniel Nordahl
 */

#include <stdio.h>
#include "Arduino.h"
#include "diag/Trace.h"
#include <LiquidCrystal.h>
#include "ArduinoJson.h"

void testJson();

LiquidCrystal lcd(8,9,4,5,6,7);

int main() {

	WDT->WDT_MR = WDT_MR_WDDIS; 		//Disable watchdog
	SystemInit();						//Set up cpu clock
	PIO_Configure(						// Enable UART
	    g_APinDescription[PINS_UART].pPort,
	    g_APinDescription[PINS_UART].ulPinType,
	    g_APinDescription[PINS_UART].ulPin,
		g_APinDescription[PINS_UART].ulPinConfiguration);
	digitalWrite(0, HIGH); // Enable pullup for RX0

	Serial.begin(9600);

	testJson();

	lcd.begin(16,2);
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
    }

}

void testJson()
{
	StaticJsonBuffer<200> jsonBuffer;
	char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
	JsonObject &root = jsonBuffer.parseObject(json);
	if (!root.success())
	{
	    Serial.println("parseObject() failed");
	}
	const char* sensor = root["sensor"];
	long time = root["time"];
	double latitude = root["data"][0];
	double longitude = root["data"][1];
	Serial.println(sensor);
	Serial.println(time);
	Serial.println(latitude, 6); // Number of decimals (6)
	Serial.println(longitude, 6);
}
