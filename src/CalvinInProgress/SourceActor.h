/*
 * SourceActor.h
 *
 *  Created on: 13 okt. 2015
 *      Author: Daniel
 */

#ifndef SRC_CALVININPROGRESS_SOURCEACTOR_H_
#define SRC_CALVININPROGRESS_SOURCEACTOR_H_
#include "../CalvinDone/CalvinMini.h"
#include "inttypes.h"

typedef struct ibuffert{
	uint32_t element[8];
	int size;
	int read;
	int write;
}intFifo;

class SourceActor{
public:
	rStatus actorInit();
	int actorCount();
	/**
	 * Actor int fifo stores 32 bit integer values.
	 * @param reference to int fifo buffer
	 * @param value to be added
	 * @return returns SUCCESS if value was added to the fifo
	 * FAIL if not.
	 */
	rStatus fifoAdd(intFifo*, uint32_t);¨
	/**
	 * Actor integer fifo stores 32 bit integer values.
	 * @param reference to integer fifo
	 * @return SUCCESS if value was popped from fifo. FAIL if fifo is empty
	 */
	uint32_t fifoPop(intFifo*);
	/**
	 * Used to determine length of fifo
	 */
	int8_t lengthOfIntData(intFifo*);
	rStatus initFifo(intFifo*);
};



#endif /* SRC_CALVININPROGRESS_SOURCEACTOR_H_ */
