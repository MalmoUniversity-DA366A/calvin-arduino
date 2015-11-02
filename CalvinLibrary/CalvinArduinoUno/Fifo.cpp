/*
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 *  Created on: 27 okt. 2015
 *      Author: Daniel Nordahl
 */
#include "Fifo.h"
extern "C" {

rStatus initFifo(fifo *fif)
{
	fif->size = FIFO_SIZE;
	fif->read = 0;
	fif->write = 0;
	return SUCCESS;
}

int8_t lengthOfData(fifo *fif)
{
	return ((fif->write - fif->read) & (fif->size -1));
}

rStatus fifoAdd(fifo *fif, int element){

	if(lengthOfData(fif) == (fif->size-1))
	{
		return FAIL;      	//fifo full;
	}
	fif->element[fif->write] = element;
	fif->write = (fif->write + 1) & (fif->size - 1);

	return SUCCESS;       //all is well
}

int fifoPop(fifo *fif){
	int ret;
	if(lengthOfData(fif) == 0)
	{
		return FAIL;    	//fifo empty
	}
	ret = fif->element[fif->read];
	fif->read = (fif->read + 1) & (fif->size - 1);
	return ret;
}


}


