
/*
 *	This file contains the arduino actors
 *	implement new actors here.
 *
 *  Created on: 26 okt. 2015
 *  Author: Daniel Nordahl
 */

#include "CalvinMini.h"
#include <LiquidCrystal.h>
#include "Arduino.h"
#include "Actors.h"

extern "C"{


int8_t actorStdOut(actor *inputActor)
{
	uint8_t inFifo;
	char tokenData[16];
	inFifo = lengthOfData(&inputActor->inportsFifo[0]);
	if(inFifo > 0)
	{
		sprintf(tokenData,"%d",(uint32_t)fifoPop(&inputActor->inportsFifo[0]));
	}
	Serial.println(tokenData);
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
	Serial.println(tokenData);


	return allOk;
}

rStatus actorInit(actor *inputActor){
	rStatus allOk = FAIL;
	fifo actorFifo;
	if(!strcmp(inputActor->type.c_str(),"io.StandardOut"))
	{
		inputActor->fire = &actorStdOut;
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
