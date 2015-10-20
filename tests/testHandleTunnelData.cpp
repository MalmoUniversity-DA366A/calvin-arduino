/*
 * testHandleTunnelData.cpp
 *
 *  Created on: 5 okt. 2015
 *      Author: Jesper
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testHandleTunnelData : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testHandleTunnelData, testSize) {
	CalvinMini calvinMini;

	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &value = jsonBuffer.createObject();

	msg["to_rt_uuid"] = "1234";
	msg["from_rt_uuid"] = "3456";
	value.set("cmd","ACK");
	msg.set("value",value);

	calvinMini.handleTunnelData(msg, reply, request);

	EXPECT_EQ(5, reply.size());
}

TEST(testHandleTunnelData, testContainsKey) {
	CalvinMini calvinMini;

	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &value = jsonBuffer.createObject();
	msg["to_rt_uuid"] = "1234";
	msg["from_rt_uuid"] = "3456";
	value.set("cmd","ACK");
	msg.set("value",value);

	calvinMini.handleTunnelData(msg, reply, request);

	EXPECT_TRUE(reply.containsKey("to_rt_uuid"));
	EXPECT_TRUE(reply.containsKey("from_rt_uuid"));
	EXPECT_TRUE(reply.containsKey("cmd"));
	EXPECT_TRUE(reply.containsKey("tunnel_id"));
	EXPECT_TRUE(reply.containsKey("value"));
}

TEST(testHandleTunnelData, testValues) {
	CalvinMini calvinMini;

	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &request = jsonBuffer.createObject();
    JsonObject &value = jsonBuffer.createObject();
	msg["to_rt_uuid"] = "1234";
	msg["from_rt_uuid"] = "3456";
	value.set("cmd","ACK");
	msg.set("value",value);

	calvinMini.handleTunnelData(msg, reply, request);

	EXPECT_STREQ("calvin-arduino", reply.get("from_rt_uuid"));
	EXPECT_STREQ("3456", reply.get("to_rt_uuid"));
	EXPECT_STREQ("TUNNEL_DATA", reply.get("cmd"));
	EXPECT_STREQ("fake-tunnel", reply.get("tunnel_id"));
	EXPECT_STREQ(NULL, reply.get("value"));
}
#endif
