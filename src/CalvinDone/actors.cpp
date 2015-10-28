
/*
 * actors.cpp
 *
 *  Created on: 26 okt. 2015
 *      Author: Daniel
 */

#include "CalvinMini.h"
#ifdef ARDUINO
#include <LiquidCrystal.h>
#include "Adafruit_PN532.h"
#include "Arduino.h"
#endif
#include "actors.h"

extern "C"{
#ifdef ARDUINO
LiquidCrystal lcdOut(52, 50, 48, 46, 44, 42);
#define PN532_IRQ   (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
//---------RFID cards/tags used: ------------
uint8_t card1[4] = {0xF1, 0x39, 0x7A, 0x0F};
uint8_t card2[4] = {0xE5, 0xA1, 0xEA, 0x45};
uint8_t tag1[4]  = {0x0B, 0x4E, 0x2E, 0x3B};
#endif

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
	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
#endif
	return standardOut(tokenData);
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
	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
#endif
	return allOk;
}

int8_t actorRFID(actor *inputActor)
{
	int8_t allOk = FAIL;
	uint32_t count;
	char tokenData[16];
	uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
	if(readRFID(uid))
	{
		count = compareMifareClassicCardUid(uid);
		allOk = fifoAdd(&inputActor->inportsFifo[0],count);
	}
	sprintf(tokenData,"%d",(uint32_t)count);
#ifdef ARDUINO
	Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
#endif
	return allOk;
}

uint8_t rfidSetup()
{
	nfc.begin();
	nfc.SAMConfig();
}

uint32_t compareMifareClassicCardUid(uint8_t *uid)
{
	uint32_t result = 0;
	if(uid[0] == card1[0] && uid[1] == card1[1] && uid[2] == card1[2] && uid[3] == card1[3])
	{
		result = 1;
	}
	else if(uid[0] == card2[0] && uid[1] == card2[1] && uid[2] == card2[2] && uid[3] == card2[3])
	{
		result = 2;
	}
	else if(uid[0] == tag1[0] && uid[1] == tag1[1] && uid[2] == tag1[2] && uid[3] == tag1[3])
	{
		result = 3;
	}
	return result;
}

uint8_t readRFID(uint8_t *uid)
{
	uint8_t result = 0;
	uint8_t uidLength;
	uint8_t success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
	if(success)
	{
		if (uidLength == 4)
		{
			result = 1;
		}
	}
	return result;
}

rStatus actorInit(actor *inputActor){
	rStatus allOk = FAIL;
	fifo actorFifo;
	if(!strcmp(inputActor->type.c_str(),"io.StandardOut"))
	{
		inputActor->fire = &actorStdOut;
	}
	else if(!strcmp(inputActor->type.c_str(),"io.RFID"))
	{
		rfidSetup();
		inputActor->fire = &actorRFID;
	}
	else
	{
		inputActor->fire = &actorCount;
	}
	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	inputActor->inportsFifo[0] = actorFifo;
	allOk = initFifo(&inputActor->inportsFifo[0]);

	return allOk;
}








}
