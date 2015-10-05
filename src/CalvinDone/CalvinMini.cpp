/*
 * This is the Calvin mini library for Arduino due
 *
 *Created on: 5 okt. 2015
 *
 */
#include <stdio.h>
#include "CalvinMini.h"
#include <inttypes.h>

actor globalActor;
fifo actorFifo;

/**
 * Current standard out is the lcd screen connected to arduino due
 */
int StdOut(){
	uint8_t inFifo;
	inFifo = globalActor.ports[0].portName[0].fifo[0].length;
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
	initFifo(&actorFifo);
	globalActor.ports[0].portName[0].fifo[0].add = &fifoAdd;
	globalActor.ports[0].portName[0].fifo[0].pop = &fifoPop;
	return 1;
}
}

/**
 * Create an new actor.
 * @param msg json list
 * @return return 1 if successful.
 */
uint8_t CalvinMini::createActor(JsonObject &msg){
	int allOk = 0;
	CalvinMini::initGlobalActor(&globalActor);
	globalActor.type = msg["type"];
	globalActor.name = msg["name"];
	globalActor.id = msg["id"];
	globalActor.fifo = msg["fifo"];
	globalActor.ports[0].key = "inport";
	globalActor.ports[0].portName[0].fifo[0].length = 0; //This is the port-fifo
	globalActor.outport = "NULL";

	actorInit();
	allOk = 1;
	return allOk;
}

/*
 * Search for keys in the actor struct. The actor struct is a struct
 * with struct arrays in it, it has 3 levels so to search it 3 keys are
 * needed.
 * @return An arrays with positions of the found keys.
 */
int8_t* CalvinMini::searchForKeys(actor* act,const char* key1,const char* key2,
		const char* key3)
{
	static int8_t keys[3] = {0,0,0};
	int i;
	for( i = 0; i < ACTOR_SIZE; i++ ){
		if(!strcmp(key1,act->ports[i].key))
		{
			keys[0] = i;
		}
	}
	for( i = 0; i < ACTOR_SIZE; i++ ){
		if(!strcmp(key2,act->ports[keys[0]].portName[i].key))
		{
			keys[1] = i;
		}
	}
	for( i = 0; i < ACTOR_SIZE; i++ ){
		if(!strcmp(key3,act->ports[keys[0]].portName[keys[1]].
				fifo[i].key))
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
void CalvinMini::initGlobalActor(actor *act){
	int8_t i,j,k;
	i = 0;
	j = 0;
	k = 0;

	for(i ; i < ACTOR_SIZE; i++)
	{
		act->ports[i].key = "null";
	}

	i = 0;
	for( i ; i < ACTOR_SIZE; i++ )
		{
		for( j ; j < ACTOR_SIZE ; j++ )
		{
			act->ports[i].portName[j].key = "null";
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
				act->ports[i].portName[j].fifo[k].key = "null";
			}
		}
	}
}
extern "C"{
/**
 * This Function initiate the fifo must be
 * called prior to using the fifo.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 */
int initFifo(fifo *fif)
{
	fif->size = FIFO_SIZE;
	fif->read = 0;
	fif->write = 0;
	return 0;
}
/**
 * Used by Add and Pop to determine fifo length.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 * @return Fifo length
 */
int lengthOfData(fifo *fif)
{
	return ((fif->write - fif->read) & (fif->size -1));
}

/**
 * Adds a new element to the fifo
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 * @return returns 0 if the fifo is full
 */
int fifoAdd(fifo *fif, const char* element){

	if(lengthOfData(fif) == (fif->size-1))
	{
		return -1;			//fifo full;
	}
	fif->element[fif->write] = element;
	fif->write = (fif->write + 1) & (fif->size - 1);

	return 0;				//all is well
}

/**
 * Return and removes the oldest element in the fifo.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 * @Return Returns fifo element, returns NULL if fifo is
 * empty.
 */
const char* fifoPop(fifo *fif){

	const char* ret;

	if(lengthOfData(fif) == 0)
	{
		return "Null";		//fifo empty
	}

	ret = fif->element[fif->read];
	fif->read = (fif->read + 1) & (fif->size - 1);

	return ret;
}

}
/**
 * Process an incomming token and add the token data to
 * an actor fifo.
 * @param Token data as a string
 * @return if data vas added to fifo this function returns
 * 1, if something went wrong it returns 0.
 */
int CalvinMini::process(const char* token){
	int allOk;
	allOk = -1;
	allOk = globalActor.ports[0].portName[0].fifo[0].add(&actorFifo,token);
	return allOk;
}

/**
 * Method for setting up a tunnel using JSON message back to Calvin-Base,
 * JSON is added to the JsonObject request that is added to the reply list.
 * @param &msg JsonObject received from Calvin-Base
 * @param &request JsonObject that is added to the "reply" list
 * @param &policy JsonObject that is an empty JsonObject
 *
 */
void CalvinMini::handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy)
{
	request.set("msg_uuid", "MSG-12345678-9101-1123-1415-161718192021");
	request.set("from_rt_uuid", "calvin-miniscule");
	request.set("to_rt_uuid", msg.get("id"));
	request.set("cmd", "TUNNEL_NEW");
	request.set("tunnel_id", "fake-tunnel");
	request.set("type", "token");
	request.set("policy", policy);
}

void loop()
{
	while(1)
	{
		// 1: Kontrollera anslutna sockets

		// 2: Fixa koppling

		// 3: Läs av meddelande

		// 4: Hantera meddelande

		// 5: Fire Actors

		// 6: Läs av utlistan

		// 7: Skicka utmeddelande
	}
}

