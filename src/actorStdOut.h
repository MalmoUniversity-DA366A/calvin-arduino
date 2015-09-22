/*
 * actorStdOut.h
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#ifndef SRC_ACTORSTDOUT_H_
#define SRC_ACTORSTDOUT_H_
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#define standardOut(x)		strlen(x)

using namespace std;
class ActorStdOut
{
public:
	int createActor(void);
	int StdOut(void);
};
struct actor{
	string type;
	string name;
	string id;
	string inport;
	string outport;
};


#endif /* SRC_ACTORSTDOUT_H_ */
