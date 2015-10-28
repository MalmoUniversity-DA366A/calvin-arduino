/*
 * actors.h
 *
 *  Created on: 26 okt. 2015
 *      Author: Daniel
 */
#ifndef CALVINDONE_ACTORS_H_
#define CALVINDONE_ACTORS_H_
#include "CalvinMini.h"
extern "C"{

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
rStatus actorInit(actor*);

/**
 * Current standard out is the lcd screen connected to arduino due
 */
int8_t actorStdOut(actor*);

/**
 * Increment the count each time the actor fires
 */
int8_t actorCount(actor*);

int8_t actorMovement(actor*);

}


#endif /* CALVINDONE_ACTORS_H_ */
