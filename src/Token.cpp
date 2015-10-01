/*
 * Token.cpp
 *
 * Author: Jesper Hansen
 */
#include "Token.h"

/**
 * Function for setting the Json reply back to Calvin-Base when the request message from
 * Calvin-Base is "Token"
 * @param msg is the JsonObject that is message from Calvin-Base
 * @param reply is the JsonObject with the reply message from Calvin-Arduino
 */
void Token::handleToken(JsonObject &msg, JsonObject &reply)
{
	reply.set("cmd", 			"TOKEN_REPLY");
	reply.set("sequencenbr", 	msg.get("sequencenbr"));
	reply.set("port_id",		msg.get("port_id"));
	reply.set("peer_port_id", 	msg.get("peer_port_id"));
	reply.set("value", 			"ACK");
}
