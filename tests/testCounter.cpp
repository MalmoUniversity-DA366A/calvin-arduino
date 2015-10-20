/*
 * testCounter.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testCounter : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testCounter, testReceiveCounter)
{
    CalvinMini mini;
    String str = "{\"to_rt_uuid\": \"calvin-miniscule\", \"from_rt_uuid\": \"422828ae-02c1-4d2f-9837-43a32fb6decb\""
                 ", \"state\": {\"prev_connections\": {\"actor_name\": \"test3:src\", \"inports\": {}, \"actor_id\": \"6bc4b467-a2d2-4254-ad84-bf918231c68e\""
                 ", \"outports\": {\"f4822772-bca4-465c-8109-a1e7a960e304\": [[\"422828ae-02c1-4d2f-9837-43a32fb6decb\", \"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\"]]}}"
                 ", \"actor_type\": \"std.Counter\", \"actor_state\": {\"count\": 117, \"name\": \"test3:src\", \"inports\": {}, \"_managed\": [\"count\", \"id\", \"name\"]"
                 ", \"outports\": {\"integer\": {\"fanout\": 1, \"name\": \"integer\", \"fifo\": {\"write_pos\": 117, \"readers\": [\"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\"]"
                 ", \"fifo\": [{\"data\": 116, \"type\": \"Token\"}, {\"data\": 117, \"type\": \"Token\"}, {\"data\": 113, \"type\": \"Token\"}"
                 ", {\"data\": 114, \"type\": \"Token\"}, {\"data\": 115, \"type\": \"Token\"}], \"N\": 5, \"tentative_read_pos\": {\"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\": 113}"
                 ", \"read_pos\": {\"98f7f8ad-f4d3-44ae-9228-7228a54c2fee3\": 113}}, \"id\": \"f4822772-bca4-465c-8109-a1e7a960e304\"}}"
                 ", \"id\": \"6bc4b467-a2d2-4254-ad84-bf918231c68e\"}}, \"cmd\": \"ACTOR_NEW\", \"msg_uuid\": \"fd71b46d-0e44-4298-9bb3-61aa0420bc45\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    mini.handleMsg(msg, reply, request);
    JsonObject &actor_state = msg["state"]["actor_state"];

    // Test if actor_type Counter is triggered
    EXPECT_STREQ("std.Counter", msg["state"]["actor_type"]);

    // Test if actor_state is store tokens
    // which should be false when actor is stdOut
    EXPECT_TRUE(actor_state.containsKey("count"));

    // Test if PORT_CONNECT is returned cmd
    // which it should be after an actor migrate
    EXPECT_STREQ("PORT_CONNECT", request["cmd"]);
}

TEST(testCounter,testCounting){
	CalvinMini mini;
	actor testActor;
	fifo testFifo;
	initFifo(&testFifo);
	testActor.inportsFifo[0] = testFifo;
	testActor.count = 0;
	EXPECT_EQ(0,testActor.count);
	actorCount(&testActor);
	EXPECT_EQ(1,testActor.count);

	testActor.count = 0;
	for(int i = 0; i < 10000; i++){
		actorCount(&testActor);
	}
	EXPECT_EQ(10000,testActor.count);
}

#endif
