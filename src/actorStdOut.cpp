/*
 * actorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "actorStdOut.h"



actor globalActor;

int ActorStdOut::StdOut(){
	const char* fifo;
	fifo = globalActor.fifo;
	return standardOut(fifo);
}

/**
 * In the current version this function has no use
 * If problem with function pointer gets resolved
 * this version of the function will be used.
 */
int ActorStdOut::actorInit(){
	//globalActor.function = &StdOut;
}
/**
 * Fires the actor with, since there is only one actor
 * with one standard out, this function will call StdOut
 * and the actor token will be printed.
 */
int ActorStdOut::actorFire(){
	int allOk = 0;
	ActorStdOut::StdOut();
	allOk = 1;
	return allOk;
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





