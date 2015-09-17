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
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

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

	// Enter a MAC address for your controller below.
	EthernetClient client;
	//client.setTimeout(5000);				//set timout to 5 seconds
	byte mac[] = {
	  0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93
	};

	Serial.begin(115200);
	lcd.begin(16,2);
    pinMode(A0,OUTPUT);
    pinMode(13,OUTPUT);
    analogWrite(A0,155);
    lcd.write("Hello World");
    lcd.setCursor(0,1);
    lcd.write("Calvin mini");

    //-------------------TEST ETHERNET----------------------------------------
    // start the Ethernet connection:
	Serial.println("here");

	// DHCP:

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
	    // no point in carrying on, so do nothing forevermore:
	  }

     //------------------------------------------------------

    while(1){				//Run loop
    	digitalWrite(13,HIGH);
    	delayMicroseconds(1000000);
    	digitalWrite(13,LOW);
    	delay(1000);
    	Serial.write(55);
    	Serial.println();

    	Serial.print("My IP address: ");
    	     for (byte thisByte = 0; thisByte < 4; thisByte++) {
    	       // print the value of each byte of the IP address:
    	       Serial.print(Ethernet.localIP()[thisByte], DEC);
    	       Serial.print(".");
    	     }
    	     Serial.println();

    }

}
