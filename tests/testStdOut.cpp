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


    mini->handleMsg(msg, reply, request, 0);

    int8_t size = mini->handleMsg(msg, reply, request, 0);

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

    // Test part of actor type name
    const char *p = msg["state"]["actor_type"].asString();
    String str2 = "";
    while(*p != '.')
    {
        str2 += *p;
        p++;
    }
    EXPECT_STREQ("io", str2.c_str());

    // Test part of actor type name
    uint8_t actorBool = !strncmp("io", msg["state"]["actor_type"].asString(), 2);
    EXPECT_EQ(1, actorBool);

    delete mini;
}
#endif
