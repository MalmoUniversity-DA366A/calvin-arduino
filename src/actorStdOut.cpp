/*
 * actorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */
#include <stdio.h>
#include "actorStdOut.h"
#include <string.h>
#include <inttypes.h>

actor globalActor;
fifo actorFifo;

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
uint8_t ActorStdOut::createActor(JsonObject &msg){
	int allOk = 0;
	globalActor.type = msg["type"];
	globalActor.name = msg["name"];
	globalActor.id = msg["id"];
	globalActor.fifo = msg["fifo"];
	globalActor.outport = "NULL";
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

TokenFifo ActorStdOut::getActorStruct(){
	return actorFifo;
}

/*
 * Search for keys in the actor struct, this version is the first
 * draft and it is in need of improvements.
 */
uint32_t* ActorStdOut::findKey(const char* key1, const char* key2, const char* key3){
	static uint32_t keys[3] = {-1,-1,-1};
	int i;
	i = 0;

	while(i < 4){
		if (!strcmp(key1,globalActor.value[i].key))
		{
			keys[0] = i;
		}
		if(!strcmp(key2,globalActor.value[i].value[i].key))
		{
			keys[1] = i;
		}
		if(!strcmp(key3,globalActor.value[i].value[i].value[i].key))
		{
			keys[2] = i;
		}
		++i;
	}

	return keys;
}

void initFifo(){
	actorFifo.add = 0;
	actorFifo.pop = 0;
}

uint32_t fifoAdd(const char* element){
	if(actorFifo.add > 5){
		actorFifo.add = 0;
	}
	actorFifo.buffer[actorFifo.add] = element;
	++(actorFifo.add);
	return 1;
}

const char* fifoPop(){
	const char* ret;
	if(actorFifo.pop == -1){
		return "NULL";				//Nothing in fifo operation failed
	}
	if(actorFifo.pop > 5){
		actorFifo.pop = 0;
	}
	ret = actorFifo.buffer[actorFifo.pop];
	++(actorFifo.pop);
	return ret;
}





