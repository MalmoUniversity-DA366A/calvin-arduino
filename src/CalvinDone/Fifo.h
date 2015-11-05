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
#include "CalvinMini.h"
#ifndef CALVINDONE_FIFO_H_
#define CALVINDONE_FIFO_H_
extern "C"{
/**
 * This Function initiate the fifo must be
 * called prior to using the fifo.
 *
 */
rStatus initFifo(fifo*);

/**
 * Adds a new element to the fifo

 * @return returns 0 if the fifo is full
 */
rStatus fifoAdd(fifo*, uint32_t);

/**
 * Return and removes the oldest element in the fifo.
 *
 * @Return Returns fifo element, returns 1 if fifo is
 * empty.
 */
uint32_t fifoPop(fifo*);

/**
 * Used by Add and Pop to determine fifo length.
 *
 * @return Fifo length
 */
int8_t lengthOfData(fifo*);

}




#endif /* CALVINDONE_FIFO_H_ */
