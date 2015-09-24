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

typedef struct actors{
	const char* type;
	const char* name;
	const char* id;
	const char* inport;
	const char* outport;
	const char* fifo;
	int (*function)();
	struct date{
		struct fifo{
			int fif;
		}fi;
	}doj;
}actor;

}

using namespace std;
class ActorStdOut
{

public:
	int createActor(JsonObject &msg);
	int createJson(void);
	actor getGlobalStruct(void);
	int actorFire(void);
};




#endif /* SRC_ACTORSTDOUT_H_ */
