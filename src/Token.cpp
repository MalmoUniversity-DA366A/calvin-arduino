/*
 * Token.cpp
 *
 *  Created on: 30 sep. 2015
 *      Author: Jesper Hansen
 */
#include "Token.h"

int Token::handleToken(JsonObject &msg, JsonObject &reply)
{
	reply.set("cmd", 			"TOKEN_REPLY");
	reply.set("sequencenbr", 	msg.get("sequencenbr"));
	reply.set("port_id",		msg.get("port_id"));
	reply.set("peer_port_id", 	msg.get("peer_port_id"));
	reply.set("value", 			"ACK");

	if (reply.success())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
