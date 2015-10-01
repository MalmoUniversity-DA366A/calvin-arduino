/*
 * actorStdOut.h
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#ifndef SRC_ACTORSTDOUT_H_
#define SRC_ACTORSTDOUT_H_
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include "../libraries/ArduinoJson/include/ArduinoJson.h"

#define standardOut(x)		strlen(x)
#define ACTOR_SIZE			5
#define QUEUE_SIZE			10
#define FIFO_SIZE			8			//Must be a power of two

extern "C"{

typedef struct buffert{
	const char* element[FIFO_SIZE];
	int size;
	int read;
	int write;
}fifo;

int initFifo(fifo*);
int fifoAdd(fifo*, const char*);
const char* fifoPop(fifo*);
int lengthOfData(fifo*);
int actorInit();

typedef struct actors{
	const char* type;
	const char* name;
	const char* id;
	const char* inport;
	const char* outport;
	const char* fifo;
	int (*function)();
	struct ports{
		const char* key;
		struct tokens{
			const char* key;
			struct fifos{
				const char* key;
				int length;
				const char* (*pop)(struct buffert*);		//Fifo pop and add
				int (*add)(struct buffert*,const char*);		//functions.
			}value[ACTOR_SIZE];
		}value[ACTOR_SIZE];
	}value[ACTOR_SIZE];
}actor;


}
using namespace std;
class ActorStdOut
{
public:
	uint8_t createActor(JsonObject &msg);
	int createJson(void);
	actor getGlobalStruct(void);
	int actorFire(void);
	uint32_t* findKey(const char*,const char*,const char*);
	int8_t* searchForKeys(const char*,const char*,const char*);
	int8_t search(const char*);
	void initGlobalActor(void);
	int process(const char*);
	int lengthOfData(fifo*);
};




#endif /* SRC_ACTORSTDOUT_H_ */
