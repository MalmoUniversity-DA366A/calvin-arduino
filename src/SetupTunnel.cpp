/*
 * setupTunnel.cpp
 *
 *  Created on: 23 sep. 2015
 *      Author: Jesper Hansen
 */

#include "SetupTunnel.h"

/**
 * Method for setting up a tunnel using JSON, JSON is added to the JsonObject reference request
 * @param &msg
 * @param &request
 * @return int
 *
 * Author: Jesper Hansen
 */
uint32_t SetupTunnel::handleSetupTunnel(JsonObject &msg, JsonObject &request)
{
	StaticJsonBuffer<1000> buffer;
	JsonObject &policy = buffer.createObject();

	const char* msg_uuid = msg["msg_uuid"];

	request["msg_uuid"] 		= "MSG-12345678-1234-5678-1234-567812345678"; // Created an hardcoded UUID
	request["from_rt_uuid"] 	= "calvin-miniscule";
	request["to_rt_uuid"] 		= msg_uuid;
	request["cmd"] 				= "TUNNEL_NEW";
	request["tunnel_id"] 		= "fake-tunnel";
	request["policy"] 			= policy;
	request["type"] 			= "token";

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
 * @param &msg
 * @param &reply
 * @return int
 *
 * Author: Jesper Hansen
 */
uint32_t SetupTunnel::handleTunnelData(JsonObject &msg, JsonObject &reply)
{
	const char* to_rt_uuid = msg["to_rt_uuid"];
	const char* from_rt_uuid = msg["from_rt_uuid"];

	reply["to_rt_uuid"] 	= from_rt_uuid;
	reply["from_rt_uuid"] 	= to_rt_uuid;
	reply["cmd"] 			= "TUNNEL_DATA";
	reply["tunnel_id"] 		= "fake-tunnel";
	reply["value"]			= "foo";				// Dummy string instead of value variable that is created from the handle_msg() function

	if (reply.success())
	{
		return 1;
	}
	else
	{
		return 2;
	}
}
