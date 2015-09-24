/*
 * actorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "actorStdOut.h"

actor globalActor;

int StdOut(){
	const char* fifo;
	fifo = globalActor.fifo;
	return standardOut(fifo);
}

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
extern "C"{
int actorInit(){
	globalActor.function = &StdOut;
}
}

/**
 * Create an new actor.
 * @param msg json list
 * @return return 1 if successful.
 */
int ActorStdOut::createActor(JsonObject &msg){
	int allOk = 0;
	globalActor.type = msg["type"];
	globalActor.name = msg["name"];
	globalActor.id = msg["id"];
	globalActor.fifo = msg["fifo"];
	globalActor.outport = "NULL";
	globalActor.doj.fi.fif;
	actorInit();
	allOk = 1;
	return allOk;
}

/**
 * Construct a json-object used in testing
 */
int ActorStdOut::createJson(){
	int allOk = 0;

	StaticJsonBuffer<2000> jsonBuffer;
	char str[] = "{\"type\":\"actor\",\"name\":\"actor1\",\"id\":\"89\",\"fifo\":\"12\"}";
	JsonObject &root = jsonBuffer.parseObject(str);

	if (root.success())
	{
			    allOk = 1;
	}

	JsonObject &p_json = root;
	ActorStdOut::createActor(p_json);
	return allOk;

}

actor ActorStdOut::getGlobalStruct(){
	return globalActor;
}





