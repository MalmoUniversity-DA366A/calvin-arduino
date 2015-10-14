/*
 * SorurceActor.cpp
 *
 *  Created on: 13 okt. 2015
 *      Author: Daniel Nordahl
 *
 */

#include "SourceActor.h"
//#include "../CalvinDone/CalvinMini.h"

actor actorTest;
intFifo testFifo;

rStatus SourceActor::actorInit(actor *inputActor)
{
	rStatus allOk = FAIL;
	allOk = initFifo(inputActor->inportsFifo[0]);
	return allOk;
}

rStatus SourceActor::actorCount(actor *inputActor)
{
	rStatus allOk = FAIL;
	uint32_t count;
	++(inputActor->count);
	count = inputActor->count;
	allOk = fifoAdd(inputActor->inportsFifo[0],count);

	return allOk;
}

rStatus SourceActor::initFifo(intFifo *fif)
{
  fif->size = FIFO_SIZE;
  fif->read = 0;
  fif->write = 0;
  return SUCCESS;
}

rStatus SourceActor::fifoAdd(intFifo *fif, uint32_t element)
{

  if(lengthOfIntData(fif) == (fif->size-1))
  {
    return FAIL;      //fifo full;
  }
  fif->element[fif->write] = element;
  fif->write = (fif->write + 1) & (fif->size - 1);

  return SUCCESS;       //all is well
}

uint32_t SourceActor::fifoPop(intFifo *fif)
{

  uint32_t ret;

  if(lengthOfIntData(fif) == 0)
  {
    return (uint32_t)FAIL;    //fifo empty
  }

  ret = fif->element[fif->read];
  fif->read = (fif->read + 1) & (fif->size - 1);

  return ret;
}

int8_t SourceActor::lengthOfIntData(intFifo *fif)
{
  return ((fif->write - fif->read) & (fif->size -1));
}
