/*
 * testHandleSetupPorts.cpp
 *
 *  Created on: 7 okt. 2015
 *      Author: Daniel Nordahl
 */

#include "../src/CalvinDone/CalvinMini.h"
#include "gtest/gtest.h"
#include "ArduinoJson.h"

class SetupPortTest : public ::testing::Test {
  protected:
   virtual void SetUp(){
   }

   virtual void TearDown(){
   }
};

/**
 * Test handleport function
 */
TEST(SetupPortTest,HandlePorts)
{
	CalvinMini mini;
	String str = "{\"to_rt_uuid\": \"calvin-arduino\", \"from_rt_uuid\": \"02fbd30d-c8a6-4cf6-b224-ea4ebdb3634b\", \"state\": {\"prev_connections\": {\"actor_name\": \"test3:snk\""
	                 ", \"inports\": {\"eca87eb7-ffde-4207-b363-31f0ab760050\": [\"02fbd30d-c8a6-4cf6-b224-ea4ebdb3634b\", \"fa46a0e5-388e-45fc-b615-b8e3ed3d9594\"]}"
	                 ", \"actor_id\": \"551cdc91-633e-4f70-954d-8e28589a8e44\", \"outports\": {}}, \"actor_type\": \"io.StandardOut\", \"actor_state\": {\"store_tokens\": false"
	                 ", \"name\": \"test3:snk\", \"inports\": {\"token\": {\"name\": \"token\", \"fifo\": {\"write_pos\": 13, \"readers\": [\"eca87eb7-ffde-4207-b363-31f0ab760050\"]"
	                 ", \"fifo\": [{\"data\": 0, \"type\": \"Token\"}, {\"data\": 0, \"type\": \"Token\"}, {\"data\": 0, \"type\": \"Token\"}, {\"data\": 0, \"type\": \"Token\"}"
	                 ", {\"data\": 0, \"type\": \"Token\"}], \"N\": 5, \"tentative_read_pos\": {\"eca87eb7-ffde-4207-b363-31f0ab760050\": 13}, \"read_pos\": {\"eca87eb7-ffde-4207-b363-31f0ab760050\": 13}}"
	                 ", \"id\": \"eca87eb7-ffde-4207-b363-31f0ab760050\"}}, \"quiet\": false, \"tokens\": [], \"_managed\": [\"tokens\", \"store_tokens\", \"quiet\", \"id\", \"name\"], \"outports\": {}"
	                 ", \"id\": \"551cdc91-633e-4f70-954d-8e28589a8e44\"}}, \"cmd\": \"ACTOR_NEW\", \"msg_uuid\": \"c8472ef4-e721-4744-88d9-2ed898952547\"}";

	StaticJsonBuffer<4096> jsonBuffer;
	JsonObject &msg = jsonBuffer.parseObject(str.c_str());
	// Create a empty JsonObject with an StaticJsonBuffer
	JsonObject &request = jsonBuffer.createObject();
	mini.handleSetupPorts(msg,request, 0);

	EXPECT_STREQ(request.get("msg_uuid"),
			"MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb7");
	EXPECT_STREQ(request.get("from_rt_uuid"),"Calvin-arduino");
	EXPECT_EQ((int)request.get("to_rt_uuid"),2);
	EXPECT_EQ((int)request.get("port_id"),0);
	EXPECT_EQ((int)request.get("peer_actor_id"),0);
	EXPECT_STREQ(request.get("cmd"),"PORT_CONNECT");

}






