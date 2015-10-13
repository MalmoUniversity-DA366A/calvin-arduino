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
	rStatus fifoAdd(intFifo*, uint32_t);
	uint32_t fifoPop(intFifo*);
	int8_t lengthOfIntData(intFifo*);
	rStatus initFifo(intFifo*);
};



#endif /* SRC_CALVININPROGRESS_SOURCEACTOR_H_ */
