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
#include "ArduinoJson.h"

#define standardOut(x)		strlen(x)

extern "C"{

int initFifo(void);
int fifoAdd(const char*);
const char* fifoPop();

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
			}value[5];
		}value[5];
	}value[5];
}actor;

typedef struct TokenFifo{
	const char *buffer[5];
	char add;
	char pop;
}fifo;

}

using namespace std;
class ActorStdOut
{

public:
	int createActor(JsonObject &msg);
	int createJson(void);
	actor getGlobalStruct(void);
	int actorFire(void);
	int* findKey(const char*,const char*,const char*);
	TokenFifo getActorStruct(void);
};




#endif /* SRC_ACTORSTDOUT_H_ */
