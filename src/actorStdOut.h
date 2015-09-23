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

struct actor{
	const char* type;
	const char* name;
	const char* id;
	const char* inport;
	const char* outport;
	const char* fifo;
};

using namespace std;
class ActorStdOut
{
public:
	int actorInit();
	int createActor(JsonObject &msg);
	int StdOut(actor inComming);
	int createJson(void);
	actor getGlobalStruct(void);
};



#endif /* SRC_ACTORSTDOUT_H_ */
