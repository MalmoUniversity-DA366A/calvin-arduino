/*
 * SourceActor.h
 *
 *  Created on: 13 okt. 2015
 *      Author: Daniel
 */

#ifndef SRC_CALVININPROGRESS_SOURCEACTOR_H_
#define SRC_CALVININPROGRESS_SOURCEACTOR_H_
//#include "../CalvinDone/CalvinMini.h"
#include "inttypes.h"

#define standardOut(x)    strlen(x)
#define ACTOR_SIZE      5
#define QUEUE_SIZE      10
#define FIFO_SIZE     8     //Must be a power of two
#define NUMBER_OF_PORTS     2

typedef enum{
  SUCCESS,
  FAIL
}rStatus;

typedef struct ibuffert{
	uint32_t element[8];
	int size;
	int read;
	int write;
}intFifo;

typedef struct actors{
  const char* type;
  const char* name;
  const char* id;
  const char* fifo;
  uint32_t count;
  int8_t (*fireActor)();
  struct ibuffert *inportsFifo[NUMBER_OF_PORTS];
  struct ibuffert *outportsFifo[NUMBER_OF_PORTS];
}actor;

class SourceActor{
public:
	rStatus actorInit(actor*);
	rStatus actorCount(actor*);
	/**
	 * Actor int fifo stores 32 bit integer values.
	 * @param reference to int fifo buffer
	 * @param value to be added
	 * @return returns SUCCESS if value was added to the fifo
	 * FAIL if not.
	 */
	rStatus fifoAdd(intFifo*, uint32_t);
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
