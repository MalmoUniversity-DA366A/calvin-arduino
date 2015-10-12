/*
 * testSendMsg.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testMessageOut : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testMessageOut, testActorNew)
{
    CalvinMini mini;
    String str = "{\"to_rt_uuid\": \"calvin-miniscule\", \"from_rt_uuid\": \"02fbd30d-c8a6-4cf6-b224-ea4ebdb3634b\", \"state\": {\"prev_connections\": {\"actor_name\": \"test3:snk\""
                 ", \"inports\": {\"eca87eb7-ffde-4207-b363-31f0ab760050\": [\"02fbd30d-c8a6-4cf6-b224-ea4ebdb3634b\", \"fa46a0e5-388e-45fc-b615-b8e3ed3d9594\"]}"
                 ", \"actor_id\": \"551cdc91-633e-4f70-954d-8e28589a8e44\", \"outports\": {}}, \"actor_type\": \"io.StandardOut\", \"actor_state\": {\"store_tokens\": false"
                 ", \"name\": \"test3:snk\", \"inports\": {\"token\": {\"name\": \"token\", \"fifo\": {\"write_pos\": 13, \"readers\": [\"eca87eb7-ffde-4207-b363-31f0ab760050\"]"
                 ", \"fifo\": [{\"data\": 0, \"type\": \"Token\"}, {\"data\": 0, \"type\": \"Token\"}, {\"data\": 0, \"type\": \"Token\"}, {\"data\": 0, \"type\": \"Token\"}"
                 ", {\"data\": 0, \"type\": \"Token\"}], \"N\": 5, \"tentative_read_pos\": {\"eca87eb7-ffde-4207-b363-31f0ab760050\": 13}, \"read_pos\": {\"eca87eb7-ffde-4207-b363-31f0ab760050\": 13}}"
                 ", \"id\": \"eca87eb7-ffde-4207-b363-31f0ab760050\"}}, \"quiet\": false, \"tokens\": [], \"_managed\": [\"tokens\", \"store_tokens\", \"quiet\", \"id\", \"name\"], \"outports\": {}"
                 ", \"id\": \"551cdc91-633e-4f70-954d-8e28589a8e44\"}}, \"cmd\": \"ACTOR_NEW\", \"msg_uuid\": \"c8472ef4-e721-4744-88d9-2ed898952547\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    int success = mini.handleMsg(msg, reply, request);

    // Test if cmd 'ACTOR_NEW' is triggered
    EXPECT_EQ(2, success);
}

TEST(testMessageOut, testToken)
{
    CalvinMini mini;
    String str = "{\"to_rt_uuid\": \"calvin-miniscule\", \"from_rt_uuid\": \"02fbd30d-c8a6-4cf6-b224-ea4ebdb3634b\", \"cmd\": \"TUNNEL_DATA\", \"value\": {\"sequencenbr\": 13, \"token\": {\"data\": 105"
                 ", \"type\": \"Token\"}, \"cmd\": \"TOKEN\", \"port_id\": \"fa46a0e5-388e-45fc-b615-b8e3ed3d9594\", \"peer_port_id\": \"eca87eb7-ffde-4207-b363-31f0ab760050\"}, \"tunnel_id\": \"fake-tunnel\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    int success = mini.handleMsg(msg, reply, request);
    JsonObject &value = msg.get("value");
    int success2 = mini.handleMsg(value,reply,request);

    JsonObject &tokenFromBase = msg.get("token");
    JsonObject &tokenInArduino = request.get("token");
    int dataFromBase = tokenFromBase.get("data");
    int dataInArduino = tokenInArduino.get("data");

    // Test if cmd 'TUNNEL_DATA' is triggered
    EXPECT_EQ(3, success);

    // Test if cmd 'TOKEN' is triggered
    EXPECT_EQ(4, success2);

    // Test if token is handled right
    EXPECT_EQ(dataFromBase, dataInArduino);
}

#endif
