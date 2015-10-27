/*
 * rfidActor.cpp
 *
 *  Created on: 27 okt. 2015
 *      Author: Andreas
 */

#include "Adafruit_PN532.h"
#include "rfidActor.h"

#define PN532_IRQ   (2)
#define PN532_RESET (3)



Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);





uint8_t rfidSetup()
{
	nfc.begin();
	nfc.SAMConfig();
}

uint32_t readRFID()
{
	uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
	uint8_t uidLength;
	uint32_t cardId;
	uint8_t success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
	nfc.PrintHex(uid, uidLength);

	if (uidLength == 4)	// We probably have a Mifare Classic card ...
	{
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
		cardId = 0;
	}
	return cardId;
}
