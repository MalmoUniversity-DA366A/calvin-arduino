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
	inFifo = lengthOfData(globalActor.inportsFifo[0]);
	if(inFifo > 0)
	{
		;//pop token
	}
	return standardOut(fifoPop(globalActor.inportsFifo[0]));
}

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
extern "C"{
int actorInit(){
	globalActor.fireActor = &StdOut;

	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	initFifo(&actorFifo);
	globalActor.inportsFifo[0] = &actorFifo;

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
	globalActor.type = msg["type"];
	globalActor.name = msg["name"];
	globalActor.id = msg["id"];

	actorInit();
	allOk = 1;
	return allOk;
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
	allOk = fifoAdd(globalActor.inportsFifo[0],token);
	return allOk;
}

/**
 * Function for setting the Json reply back to Calvin-Base when the request message from
 * Calvin-Base is "Token"
 * @param msg is the JsonObject that is message from Calvin-Base
 * @param reply is the JsonObject with the reply message from Calvin-Arduino
 */
void CalvinMini::handleToken(JsonObject &msg, JsonObject &reply)
{
	process(msg.get("token"));
	reply.set("cmd", 			"TOKEN_REPLY");
	reply.set("sequencenbr", 	msg.get("sequencenbr"));
	reply.set("port_id",		msg.get("port_id"));
	reply.set("peer_port_id", 	msg.get("peer_port_id"));
	reply.set("value", 			"ACK");
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

/**
 * Method for handle the tunnel data using JSON, JSON is added to the JsonObject reference reply
 * @param &msg JsonObject received from Calvin-Base
 * @param &reply JsonObject that is added to the "reply" list
 *
 * Author: Jesper Hansen
 */
void CalvinMini::handleTunnelData(JsonObject &msg, JsonObject &reply)
{
	reply.set("to_rt_uuid", 	msg.get("from_rt_uuid"));
	reply.set("from_rt_uuid", 	msg.get("to_rt_uuid"));
	reply.set("cmd", 			"TUNNEL_DATA");
	reply.set("tunnel_id",		""); // None in python
	reply.set("value",			"foo"); // Look in Calvin-Mini.py
}

/**
 * Handle all different messages
 * @param msg JsonObject
 * @param reply JsonObject
 * @param request JsonObject
 */
int CalvinMini::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, JsonObject &policy)
{
  if(!strcmp(msg.get("cmd"),"JOIN_REQUEST"))
  {
	  return 1;
  }
  else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
  {
	  return 2;
  }
  else if(!strcmp(msg.get("cmd"),"TUNNEL_DATA"))
  {
	  return 3;
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN"))
  {
	  return 4;
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
  {
	  return 5;
  }
  else if(!strcmp(msg.get("cmd"),"REPLY"))
  {
	  return 6;
  }
  else
  {

      standardOut("UNKNOWN CMD");

	  return 7;
  }
}

void loop()
{
	while(1)
	{
		// 1: Kontrollera anslutna sockets

		// 2: Fixa koppling

		// 3: L�s av meddelande

		// 4: Hantera meddelande

		// 5: Fire Actors

		// 6: L�s av utlistan

		// 7: Skicka utmeddelande
	}
}

