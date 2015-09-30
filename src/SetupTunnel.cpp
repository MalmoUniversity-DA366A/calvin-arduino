/*
 * setupTunnel.cpp
 *
 *  Created on: 23 sep. 2015
 *      Author: Jesper Hansen
 */

#include "SetupTunnel.h"

/**
 * Method for setting up a tunnel using JSON message back to Calvin-Base,
 * JSON text is added to the JsonObject request that is added to the reply list.
 * @param &msg JsonObject received from Calvin-Base
 * @param &request JsonObject that is added to the "reply" list
 * @return uint32_t just for checking if the request Json is created correctly
 *
 * Author: Jesper Hansen
 */
uint32_t SetupTunnel::handleSetupTunnel(JsonObject &msg, JsonObject &request)
{
	StaticJsonBuffer<1000> buffer;
	JsonObject &policy = buffer.createObject();

	request.set("from_rt_uuid", 	"calvin-miniscule");
	request.set("cmd", 				"TUNNEL_NEW");
	request.set("tunnel_id",  		"fake-tunnel");
	request.set("to_rt_uuid",  		msg.get("msg_uuid"));
	request.set("policy", 			policy);
	request.set("type",				"token");
	request.set("msg_uuid", 		"MSG-12345678-9101-1123-1415-161718192021");

	if (request.success())
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

/**
 * Method for handle the tunnel data using JSON, JSON is added to the JsonObject reference reply
 * @param &msg JsonObject received from Calvin-Base
 * @param &reply JsonObject that is added to the "reply" list
 * @return int just for checking if the request Json is created correctly
 *
 * Author: Jesper Hansen
 */
uint32_t SetupTunnel::handleTunnelData(JsonObject &msg, JsonObject &reply)
{
	reply.set("to_rt_uuid", 	msg.get("from_rt_uuid"));
	reply.set("from_rt_uuid", 	msg.get("to_rt_uuid"));
	reply.set("cmd", 			"TUNNEL_DATA");
	reply.set("tunnel_id",		"fake-tunnel");
	reply.set("value",			"foo");

	if (reply.success())
	{
		return 1;
	}
	else
	{
		return 2;
	}
}
