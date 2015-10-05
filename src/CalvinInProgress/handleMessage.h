/*
 * handleMessage.h
 *
 *  Created on: 27 sep. 2015
 *      Author: Jesper Hansen
 */

#ifndef HANDLEMESSAGE_H_
#define HANDLEMESSAGE_H_

#include "ArduinoJson.h"

// struct that acts as an Linked List
typedef struct node {
	JsonObject val;
	struct node * next;
}nodes;

class MessageHandler {
public:
	int handleMsg(JsonObject &msg);
};



#endif /* HANDLEMESSAGE_H_ */
