/*
 * actorStdOut.h
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#ifndef SRC_ACTORSTDOUT_H_
#define SRC_ACTORSTDOUT_H_

class ActorStdOut
{
public:
	int createActor(void);
};

struct actor{
	char type;
	char name;
	char id;
	char inport;
	char outport;
};


#endif /* SRC_ACTORSTDOUT_H_ */
