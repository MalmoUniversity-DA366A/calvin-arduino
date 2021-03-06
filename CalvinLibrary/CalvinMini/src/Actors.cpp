
/*
 *	This file contains the Calvin Arduino actors
 *	implement new actors here.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015
 *
 * Andreas Elvstam, Jesper Hansen, Peter Johansson, Daniel Nordahl, Carl �str�m
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

#include "CalvinMini.h"
#include <LiquidCrystal.h>
#include "Adafruit_PN532.h"
#include "Arduino.h"
#include "Actors.h"

extern "C"{
LiquidCrystal lcdOut(52, 50, 48, 46, 44, 42);
#define PN532_IRQ   (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
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
// ---------PINs used by Range sensor -------
#define echoPin 28
#define trigPin 30
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
	Serial.print("Std:   ");
	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
	return standardOut(tokenData);
}

int8_t movementStd(actor *inputActor)
{
	int8_t inFifo;
	inFifo = lengthOfData(&inputActor->inportsFifo[0]);
	if(inFifo > 0 && fifoPop(&inputActor->inportsFifo[0]))
	{
		digitalWrite(31,HIGH);
	}else{
		digitalWrite(31,LOW);
	}
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
	Serial.print("Count:   ");
	Serial.println(tokenData);
	return allOk;
}

int8_t actorMovement(actor *inputActor)
{
  int8_t allOk = FAIL;
  uint8_t detection;
  char tokenData[16];
  detection = digitalRead(22);
  delay(50);
  allOk = fifoAdd(&inputActor->inportsFifo[0],detection);
  sprintf(tokenData,"%d",(uint8_t)detection);
  Serial.println(tokenData);
  lcdOut.clear();
  lcdOut.write(tokenData);

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

	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);

	return allOk;
}
int8_t actorSonicRange(actor *inputActor)
{
	int8_t allOk = FAIL;
	uint32_t distance;
	uint32_t result;
	char tokenData[16];


	digitalWrite(trigPin, LOW);
	delay(5);
	digitalWrite(trigPin, HIGH);
	delay(10);
	digitalWrite(trigPin, LOW);

	distance = pulseIn(echoPin, HIGH, 1000000L); //Read ultrasonic reflection
	delay(50);
	distance = (distance/58);


	if(distance<=120 && distance>=80)
	{
		result = 1;
	}
	else if(distance<=70 && distance>=50)
	{
		result = 2;
	}
	else if(distance<40 && distance>=10)
	{
		result = 3;
	}else if(distance>150 && distance <200)
	{
		result = 4;
	}
	else
	{
		result = 0;
	}

	allOk = fifoAdd(&inputActor->inportsFifo[0],result);
	sprintf(tokenData,"%d",(uint32_t)result);
	return allOk;
}

void setupSonicRange()
{
	pinMode(echoPin, INPUT);
	pinMode(trigPin, OUTPUT);
}

uint8_t rfidSetup()
{
	nfc.begin();
	uint8_t result = nfc.SAMConfig();
	return result;
}

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
	uint16_t timeout = 50;				//50 millis, to ensure fast reading from RFID shield

	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);

	if(success)
	{
		delay(timeout);					//to minimize multiple reads of the same card.
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
		controlLed(count);

		result = 1;
	}
	sprintf(tokenData,"%d",(uint32_t)count);

	return result;
}

void controlLed(uint32_t id)
{
	switch(id)
	{
		case(1):
			digitalWrite(LED_RED, HIGH);
			break;
		case(2):
			digitalWrite(LED_YELLOW, HIGH);
			break;
		case(3):
			digitalWrite(LED_GREEN, HIGH);
			break;
		case(4):
			digitalWrite(LED_RED, LOW);
			digitalWrite(LED_YELLOW, LOW);
			digitalWrite(LED_GREEN, LOW);
			break;
		default:
			break;
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

		rfidSetup();

		inputActor->fire = &actorRFID;
	}
	else if(!strcmp(inputActor->type.c_str(),"std.SonicRangeSensor"))
	{
		setupSonicRange();
		inputActor->fire = &actorSonicRange;
	}
	else if(!strcmp(inputActor->type.c_str(),"io.LEDStandardOut"))
	{
		setupLedOut();
		inputActor->fire = &actorLED;
	}
	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	inputActor->inportsFifo[0] = actorFifo;
	allOk = initFifo(&inputActor->inportsFifo[0]);

	return allOk;
}

}
