/*
 * testStdOut.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testStdOut : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testStdOut, testStandardOut)
{
    CalvinMini *mini = new CalvinMini;
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
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();


    mini->handleMsg(msg, reply, request);

    int8_t size = mini->handleMsg(msg, reply, request);

    JsonObject &actor_state = msg["state"]["actor_state"];

    // Test if number of outgoing messages is 2
    EXPECT_EQ(size,2);

    // Test if actor_type stdOut is triggered
    EXPECT_STREQ("io.StandardOut", msg["state"]["actor_type"]);

    // Test if actor_state contains store tokens
    // which should be true when actor is stdOut
    EXPECT_TRUE(actor_state.containsKey("store_tokens"));

    // Test if PORT_CONNECT is returned cmd
    // which it should be after an actor migrate
    EXPECT_STREQ("PORT_CONNECT", request["cmd"]);
    delete mini;
}
TEST(testStdOut, testTunnel)
{
    CalvinMini mini;
    String str = "{\"to_rt_uuid\": \"calvin-arduino\", \"from_rt_uuid\": \"cf44176a-4e54-4afc-9fc9-af3bbd00b68d\","
            "\"cmd\": \"TUNNEL_DATA\", \"value\": {\"sequencenbr\": 101, \"token\": {\"data\": 5253, \"type\": \"Token\"}, "
            "\"cmd\": \"TOKEN\", \"port_id\": \"9b78c08e-89d0-47e8-b991-7a85b60fcec8\", \"peer_port_id\": \"c06f556d-3b60-4b08-8f28-de35fa4f99bf\"}, \"tunnel_id\": \"fake-tunnel\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    int8_t size = mini.handleMsg(msg, reply, request);

    // Test if number of outgoing messages is 2
    EXPECT_EQ(size,1);
}
#endif
