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

/**
 * Current standard out is the lcd screen connected to arduino due
 */
int StdOut(){
	uint8_t inFifo;
	inFifo = globalActor.value[0].value[0].value[0].length;
	if(inFifo > 0)
	{
		;//pop token
	}
	return standardOut(globalActor.fifo);
}

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
extern "C"{
int actorInit(){
	globalActor.function = &StdOut;

	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	globalActor.value[0].value[0].value[0].add = &fifoAdd;
	globalActor.value[0].value[0].value[0].pop = &fifoPop;
	return 1;
}
}

/**
 * Create an new actor.
 * @param msg json list
 * @return return 1 if successful.
 */
uint8_t ActorStdOut::createActor(JsonObject &msg){
	int allOk = 0;
	ActorStdOut::initGlobalActor();
	globalActor.type = msg["type"];
	globalActor.name = msg["name"];
	globalActor.id = msg["id"];
	globalActor.fifo = msg["fifo"];
	globalActor.value[0].key = "inport";
	globalActor.value[0].value[0].value[0].length = 0; //This is the port-fifo
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

actor ActorStdOut::getGlobalStruct()
{
	return globalActor;
}

/*
 * Search for keys in the actor struct. The actor struct is a struct
 * with struct arrays in it, it has 3 levels so to search it 3 keys are
 * needed.
 * @return An arrays with positions of the found keys.
 */
int8_t* ActorStdOut::searchForKeys(const char* key1,const char* key2,
		const char* key3)
{
	static int8_t keys[3] = {0,0,0};
	int i;
	for( i = 0; i < ACTOR_SIZE; i++ ){
		if(!strcmp(key1,globalActor.value[i].key))
		{
			keys[0] = i;
		}
	}
	for( i = 0; i < ACTOR_SIZE; i++ ){
		if(!strcmp(key2,globalActor.value[keys[0]].value[i].key))
		{
			keys[1] = i;
		}
	}
	for( i = 0; i < ACTOR_SIZE; i++ ){
		if(!strcmp(key3,globalActor.value[keys[0]].value[keys[1]].
				value[i].key))
		{
			keys[2] = i;
		}
	}
	return keys;
}
/**
 * To be able to search thru the struct all keys
 * needs a value, this method sets it to null.
 */
void ActorStdOut::initGlobalActor(){
	int8_t i,j,k;
	i = 0;
	j = 0;
	k = 0;

	for(i ; i < ACTOR_SIZE; i++)
	{
		globalActor.value[i].key = "null";
	}

	i = 0;
	for( i ; i < ACTOR_SIZE; i++ )
		{
		for( j ; j < ACTOR_SIZE ; j++ )
		{
			globalActor.value[i].value[j].key = "null";
		}
		}
	i = 0;
	j = 0;

	for( i ; i < ACTOR_SIZE ; i++ )
	{
		for( j ; j < ACTOR_SIZE ; j++)
		{
			for( k ; k < ACTOR_SIZE ; k++)
			{
				globalActor.value[i].value[j].value[k].key = "null";
			}
		}
	}
}

/**
 * This Function initiate the fifo must be
 * called prior to using the fifo.
 */
void initFifo(){
	actorFifo.add = 0;
	actorFifo.pop = 0;
}
/**
 * Adds a new element to the fifo
 * @return returns 0 if the fifo is full
 */
int fifoAdd(const char* element){

	if(actorFifo.add == ((actorFifo.pop - 1 + QUEUE_SIZE) % QUEUE_SIZE)){
		return -1;
	}
	actorFifo.buffer[actorFifo.add] = element;
	(++actorFifo.add);

	return 1;
}

/**
 * Return and removes the oldest element in the fifo.
 * @Return Returns fifo element, returns NULL if fifo is
 * empty.
 */
const char* fifoPop(){

	const char* ret;
	if(actorFifo.add == actorFifo.pop)
	{
		return "NULL";				//Fifo full
	}

	ret = actorFifo.buffer[actorFifo.pop];
	(++actorFifo.pop);
	return ret;
}
/**
 * Process an incomming token and add the token data to
 * an actor fifo.
 * @param Token data as a string
 * @return if data vas added to fifo this function returns
 * 1, if something went wrong it returns 0.
 */
int ActorStdOut::process(const char* token){
	int allOk;
	allOk = 0;
	allOk = globalActor.value[0].value[0].value[0].add(token);
	return allOk;
}





