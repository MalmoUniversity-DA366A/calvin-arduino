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

	const char* msg_uuid = msg["msg_uuid"];

	request["from_rt_uuid"] 	= "calvin-miniscule"; 							// Runtime ID
	request["cmd"] 				= "TUNNEL_NEW";									// Command
	request["tunnel_id"] 		= "fake-tunnel";								// Tunnel ID
	request["to_rt_uuid"] 		= msg_uuid;										// Destination Runtime ID
	request["policy"] 			= policy;										// Unused, create an empty JsonObject
	request["type"] 			= "token";										// What will be tunneled
	request["msg_uuid"] 		= "MSG-12345678-1234-5678-1234-567812345678"; 	// Message ID <Created an hardcoded UUID>

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
