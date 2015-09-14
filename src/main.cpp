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
void printJson(String str);
String jsonSerialize(char *temp);
LiquidCrystal lcd(8,9,4,5,6,7);
#define MAX_LENGTH 255
#define TERMINATOR 0x0A // $0A in Terminal.exe
int main() {

	WDT->WDT_MR = WDT_MR_WDDIS; 		//Disable watchdog
	SystemInit();						//Set up cpu clock
	PIO_Configure(						// Enable UART
	    g_APinDescription[PINS_UART].pPort,
	    g_APinDescription[PINS_UART].ulPinType,
	    g_APinDescription[PINS_UART].ulPin,
		g_APinDescription[PINS_UART].ulPinConfiguration);
	digitalWrite(0, HIGH); // Enable pullup for RX0

	Serial.begin(115200);
	char temp[MAX_LENGTH+1]; // Make room for NULL terminator
	String str = "";

	int size = Serial.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
	temp[size] = '\0';
	Serial.println(temp); // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
	if(size)
	{
		str = jsonSerialize(temp);
		printJson(str); // Prints: {"sensor":"gps","time":"flies"}
	}
	else
		testJson();

	// Test for equality between predefined and user input
	char json[] = "{\"sensor\":\"gps\",\"time\":\"flies\"}";
	printJson(json); // Prints: {"sensor":"gps","time":"flies"}
/*
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
    }*/

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

void printJson(String str)
{
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(str);
	if(!root.success())
	{
		Serial.println("parseObject() failed");
	}
	root.printTo(Serial);
	Serial.println();
}

String jsonSerialize(char *temp)
{
	String str = "";
	int count = 0;
	while(temp[count] != '\0')
	{
		if(temp[count+1] == '\"')
		{
			count++;
		}
		str += temp[count];
		count++;
	}
	return str;
}
