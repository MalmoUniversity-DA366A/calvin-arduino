/*
 * actorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "actorStdOut.h"

actor globalActor;

int ActorStdOut::StdOut(){
	return standardOut("Hej");
}

/**
 * Create an new actor.
 * @param msg json list
 * @return return 1 if successful.
 */
int ActorStdOut::createActor(){
	int allOk = 0;
	actor newActor;

	newActor.type = "NULL";
	newActor.name = "NULL";
	newActor.id = "NULL";
	newActor.inport = "NULL";
	newActor.outport = "NULL";

	globalActor = newActor;

	allOk = 1;

	return allOk;
}





