
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
#include "Arduino.h"
#endif
#include "Actors.h"

extern "C"{
#ifdef ARDUINO
LiquidCrystal lcdOut(52, 50, 48, 46, 44, 42);
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

void movementStd(actor *inputActor)
{
#ifdef ARDUINO
	int8_t inFifo;
	inFifo = lengthOfData(&inputActor->inportsFifo[0]);
	if(inFifo > 0 && fifoPop(&inputActor->inportsFifo[0]))
	{
		digitalWrite(24,HIGH);
	}else{
		digitalWrite(24,LOW);
	}
#endif
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

int8_t actorMovement(actor *inputActor)
{
  int8_t allOk = FAIL;
  uint8_t detection;
  char tokenData[16];
  detection = digitalRead(22);
  delay(50);
  allOk = fifoAdd(&inputActor->inportsFifo[0],detection);
  sprintf(tokenData,"%d",(uint8_t)detection);
#ifdef ARDUINO
  Serial.println(tokenData);
  lcdOut.clear();
  lcdOut.write(tokenData);
#endif
  return allOk;
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
	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	inputActor->inportsFifo[0] = actorFifo;
	allOk = initFifo(&inputActor->inportsFifo[0]);

	return allOk;
}








}