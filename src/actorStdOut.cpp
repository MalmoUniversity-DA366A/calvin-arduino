/*
 * actorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "actorStdOut.h"


actor globalActor;

int ActorStdOut::StdOut(actor inComming){

	return standardOut(inComming.fifo);
}

int ActorStdOut::actorInit(){

}

/**
 * Create an new actor.
 * @param msg json list
 * @return return 1 if successful.
 */
int ActorStdOut::createActor(JsonObject &msg){
	int allOk = 0;
	globalActor.type = msg["type"];
	globalActor.name = msg["type1"];
	globalActor.id = msg["id"];
	globalActor.fifo = "Test me";
	globalActor.outport = msg["port"];
	allOk = 1;

	return allOk;
}

int ActorStdOut::createJson(){
	int allOk = 0;

	StaticJsonBuffer<2000> jsonBuffer;
	char str[] = "{\"type\":\"actor\",\"type1\":\"actor1\",\"id\":\"actor2\",\"port\":\"12\"}";
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





