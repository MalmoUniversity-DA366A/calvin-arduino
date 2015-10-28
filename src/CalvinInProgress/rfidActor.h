/*
 * rfidActor.h
 *
 *  Created on: 27 okt. 2015
 *      Author: Andreas
 */

#ifndef CALVININPROGRESS_RFIDACTOR_H_
#define CALVININPROGRESS_RFIDACTOR_H_



uint8_t rfidSetup(void);
String compareMifareClassicCardUid(uint8_t *uid);
uint8_t readRFID(uint8_t *uid);
void rfidTestLoop(void);

#endif /* CALVININPROGRESS_RFIDACTOR_H_ */
