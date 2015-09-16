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
#include <Ethernet.h>

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
	byte mac[] = {
	  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
	};

	Serial.begin(115200);
	/*
	lcd.begin(16,2);
    pinMode(A0,OUTPUT);
    pinMode(13,OUTPUT);
    analogWrite(A0,155);
    lcd.write("Hello World");
    lcd.setCursor(0,1);
    lcd.write("Calvin mini");
	 */
    //-------------------TEST ETHERNET----------------------------------------
    // start the Ethernet connection:
     if (Ethernet.begin(mac) == 0)
     {
    	 Serial.println("Failed to configure Ethernet using DHCP");
       // no point in carrying on, so do nothing forevermore:
       //for (;;)
       //  ;
     }

     // print your local IP address:
     Serial.print("My IP address: ");
     for (byte thisByte = 0; thisByte < 4; thisByte++) {
       // print the value of each byte of the IP address:
       Serial.print(Ethernet.localIP()[thisByte], DEC);
       Serial.print(".");
     }
     Serial.println();

     //------------------------------------------------------

    while(1){				//Run loop
    	digitalWrite(13,HIGH);
    	delayMicroseconds(1000000);
    	digitalWrite(13,LOW);
    	delay(1000);
    	Serial.write(55);
    	Serial.println();

    }

}
