/*
 * actors.h
 *
 *  Created on: 26 okt. 2015
 *      Author: Daniel
 */
#ifndef CALVINDONE_ACTORS_H_
#define CALVINDONE_ACTORS_H_
#include "CalvinMini.h"
extern "C"{

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
rStatus actorInit(actor*);

/**
 * Current standard out is the lcd screen connected to arduino due
 */
int8_t actorStdOut(actor*);

/**
 * Increment the count each time the actor fires
 */
int8_t actorCount(actor*);

/**
 * Reads RFID-cards
 */
int8_t actorRFID(actor*);

/**
 * Setup the RFID reader
 * @Return uint8_t 1 if success, else 0.
 */
uint8_t rfidSetup(void);

/**
 * Determine what card was read.
 * @Param uint8_t* uid, the UID to compare
 * @Return uint32_t the value matching the UID. If none found 0.
 */
uint32_t compareMifareClassicCardUid(uint8_t*);

/**
 * Reads an RFID card/tag UID.
 * @Param uint8_t* uid, the array to store the read UID.
 * @Return int8_t returns 1 if successful read, else 0.
 */
uint8_t readRFID(uint8_t*);

/**
 *
 */
uint32_t controlLed(uint32_t);

/**
 *
 */
void setupLedOut(void);

}


#endif /* CALVINDONE_ACTORS_H_ */
