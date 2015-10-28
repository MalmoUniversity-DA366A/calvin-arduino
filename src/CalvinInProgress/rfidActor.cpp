/*
 * rfidActor.cpp
 *
 *  Created on: 27 okt. 2015
 *      Author: Andreas
 */


#include "Adafruit_PN532.h"
#include "rfidActor.h"
#include <stdio.h>
#include <inttypes.h>

#define PN532_IRQ   (2)
#define PN532_RESET (3)
/*
		KORT 1:
UID Value: 0xF1 0x39 0x7A 0x0F
card #4047075855
		KORT 2:
UID Value: 0xE5 0xA1 0xEA 0x45
card #3852593733
		TAGG:
UID Value: 0x0B 0x4E 0x2E 0x3B
card #189673019
*/

uint8_t card1[4] = {0xF1, 0x39, 0x7A, 0x0F};
uint8_t card2[4] = {0xE5, 0xA1, 0xEA, 0x45};
uint8_t tag1[4]  = {0x0B, 0x4E, 0x2E, 0x3B};

Adafruit_PN532 nfcTest(PN532_IRQ, PN532_RESET);


uint8_t rfidSetup()
{
	nfcTest.begin();
	nfcTest.SAMConfig();
}

String compareMifareClassicCardUid(uint8_t *uid)
{
	String result = "Unknown Card";
	if(uid[0] == card1[0] && uid[1] == card1[1] && uid[2] == card1[2] && uid[3] == card1[3])
	{
		result = "Card 1";
	}
	else if(uid[0] == card2[0] && uid[1] == card2[1] && uid[2] == card2[2] && uid[3] == card2[3])
	{
		result = "Card 2";
	}
	else if(uid[0] == tag1[0] && uid[1] == tag1[1] && uid[2] == tag1[2] && uid[3] == tag1[3])
	{
		result = "Tag 1";
	}
	return result;
}

uint8_t readRFID(uint8_t *uid)
{
	uint8_t result = 0;
	uint8_t uidLength;
	uint32_t cardId;
	uint8_t success = nfcTest.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
	nfcTest.PrintHex(uid, uidLength);

	if (uidLength == 4)	// We probably have a Mifare Classic card ...
	{
		result = 1;
		cardId = uid[0];
		cardId <<= 8;
		cardId |= uid[1];
		cardId <<= 8;
		cardId |= uid[2];
		cardId <<= 8;
		cardId |= uid[3];
		Serial.print("Seems to be a Mifare Classic card #");
		Serial.println(cardId);
	}
	else	//not a Mifare classic card, maybe add other card types?
	{

	}
	return result;
}

void rfidTestLoop()
{
	rfidSetup();
	while(1)
	{
		uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
		if(readRFID(uid))
		{
			String str = compareMifareClassicCardUid(uid);
			Serial.println(str);
			Serial.println();
		}
	}
}
