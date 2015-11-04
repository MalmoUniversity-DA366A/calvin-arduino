
/*
 *	This file contains the arduino actors
 *	implement new actors here.
 *
 *  Created on: 26 okt. 2015
 *  Author: Daniel Nordahl
 */

#include "CalvinMini.h"
#ifdef ARDUINO
#include <LiquidCrystal.h>
#include "Adafruit_PN532.h"
#include "Arduino.h"
#endif
#include "Actors.h"

extern "C"{
#ifdef ARDUINO
LiquidCrystal lcdOut(52, 50, 48, 46, 44, 42);
#define PN532_IRQ   (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
#endif
//---------RFID cards/tags used: ------------
uint8_t card1[4] = {0xF1, 0x39, 0x7A, 0x0F};
uint8_t card2[4] = {0xE5, 0xA1, 0xEA, 0x45};
uint8_t tag1[4]  = {0x0B, 0x4E, 0x2E, 0x3B};
//-------------------------------------------
//----------PINs used for LEDs: -------------
#define LED_RED		22
#define LED_YELLOW	24
#define LED_GREEN	26
//-------------------------------------------


int8_t actorStdOut(actor *inputActor)
{
	uint8_t inFifo;
	char tokenData[16];
	inFifo = lengthOfData(&inputActor->inportsFifo[0]);
	if(inFifo > 0)
	{
		sprintf(tokenData,"%d",(uint32_t)fifoPop(&inputActor->inportsFifo[0]));
	}
#ifdef ARDUINO
	Serial.print("Std:   ");
	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
#endif
	return standardOut(tokenData);
}

int8_t movementStd(actor *inputActor)
{
#ifdef ARDUINO
	int8_t inFifo;
	inFifo = lengthOfData(&inputActor->inportsFifo[0]);
	if(inFifo > 0 && fifoPop(&inputActor->inportsFifo[0]))
	{
		digitalWrite(31,HIGH);
	}else{
		digitalWrite(31,LOW);
	}
#endif
	return 0;
}

int8_t actorCount(actor *inputActor)
{
	int8_t allOk = FAIL;
	uint32_t count;
	char tokenData[16];
	++(inputActor->count);
	count = inputActor->count;
	allOk = fifoAdd(&inputActor->inportsFifo[0],count);
	sprintf(tokenData,"%d",(uint32_t)count);
#ifdef ARDUINO
	Serial.print("Count:   ");
	Serial.println(tokenData);
#endif
	return allOk;
}

int8_t actorMovement(actor *inputActor)
{
  int8_t allOk = FAIL;
  uint8_t detection;
  char tokenData[16];
#ifdef ARDUINO
  detection = digitalRead(22);
  delay(50);
#endif
  allOk = fifoAdd(&inputActor->inportsFifo[0],detection);
  sprintf(tokenData,"%d",(uint8_t)detection);
#ifdef ARDUINO
  Serial.println(tokenData);
  lcdOut.clear();
  lcdOut.write(tokenData);
#endif
  return allOk;
}

int8_t actorRFID(actor *inputActor)
{
	int8_t allOk = FAIL;
	uint32_t count = 0;
	char tokenData[16];
	uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
	if(readRFID(uid) == 1)
	{
		count = compareMifareClassicCardUid(uid);
		allOk = fifoAdd(&inputActor->inportsFifo[0],count);
	}
	else
	{
		allOk = fifoAdd(&inputActor->inportsFifo[0], 0);
	}

	sprintf(tokenData,"%d",(uint32_t)count);
#ifdef ARDUINO
	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
#endif
	return allOk;
}
#ifdef ARDUINO
uint8_t rfidSetup()
{
	nfc.begin();
	uint8_t result = nfc.SAMConfig();
	return result;
}
#endif
uint32_t compareMifareClassicCardUid(uint8_t *uid)
{
	uint32_t result;
	if(uid[0] == card1[0] && uid[1] == card1[1] && uid[2] == card1[2] && uid[3] == card1[3])		// Card 1
	{
		result = 1;
	}
	else if(uid[0] == card2[0] && uid[1] == card2[1] && uid[2] == card2[2] && uid[3] == card2[3])	// Card 2
	{
		result = 2;
	}
	else if(uid[0] == tag1[0] && uid[1] == tag1[1] && uid[2] == tag1[2] && uid[3] == tag1[3])		// Tag 1
	{
		result = 3;
	}
	else
	{
		result = 4;
	}
	return result;
}

uint8_t readRFID(uint8_t *uid)
{
	uint8_t success = 0;
	uint8_t result = 0;
	uint8_t uidLength;
	uint16_t timeout = 50;				//50 millis
#ifdef ARDUINO
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);
#else
	success = 1;
#endif
	if(success)
	{
		if (uidLength == 4)
		{
			result = 1;
		}
	}
	return result;
}

int8_t actorLED(actor *inputActor)
{
	uint8_t result = 0;
	uint8_t inFifo;
	uint32_t count = 0;
	char tokenData[16];
	inFifo = lengthOfData(&inputActor->inportsFifo[0]);
	if(inFifo > 0)
	{
		count = fifoPop(&inputActor->inportsFifo[0]);
#ifdef ARDUINO
		controlLed(count);
#endif
		result = 1;
	}

#ifdef ARDUINO
	sprintf(tokenData,"%d",(uint32_t)count);
#endif
	return result;
}
#ifdef ARDUINO
uint32_t controlLed(uint32_t id)
{
	switch(id)
	{
		case(0):
			return id;
		case(1):
			digitalWrite(LED_RED, HIGH);
			return id;
		case(2):
			digitalWrite(LED_YELLOW, HIGH);
			return id;
		case(3):
			digitalWrite(LED_GREEN, HIGH);
			return id;
		case(4):
			digitalWrite(LED_RED, LOW);
			digitalWrite(LED_YELLOW, LOW);
			digitalWrite(LED_GREEN, LOW);
			return id;
		default:
			return 255;
	}
}

void setupLedOut()
{
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_YELLOW, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_YELLOW, LOW);
	digitalWrite(LED_GREEN, LOW);
}
#endif

rStatus actorInit(actor *inputActor){
	rStatus allOk = FAIL;
	fifo actorFifo;
	if(!strcmp(inputActor->type.c_str(),"io.StandardOut"))
	{
		inputActor->fire = &actorStdOut;
	}
	else if(!strcmp(inputActor->type.c_str(),"std.Counter"))
	{
		inputActor->fire = &actorCount;
	}
	else if(!strcmp(inputActor->type.c_str(),"std.MovementSensor"))
	{
	  inputActor->fire = &actorMovement;
	}
	else if(!strcmp(inputActor->type.c_str(),"io.MovementStandardOut"))
	{
	   inputActor->fire = &movementStd;
	}
	else if(!strcmp(inputActor->type.c_str(),"std.RFID"))
	{
#ifdef ARDUINO
		rfidSetup();
#endif
		inputActor->fire = &actorRFID;
	}
	else if(!strcmp(inputActor->type.c_str(),"io.LEDStandardOut"))
	{
#ifdef ARDUINO
		setupLedOut();
#endif
		inputActor->fire = &actorLED;
	}
	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	inputActor->inportsFifo[0] = actorFifo;
	allOk = initFifo(&inputActor->inportsFifo[0]);

	return allOk;
}

}
