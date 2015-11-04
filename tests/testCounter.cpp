
/*
 * testCounter.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"
#include "../src/CalvinDone/Actors.h"

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
                 ", \"fifo\": [{\"data\": 113, \"type\": \"Token\"}, {\"data\": 114, \"type\": \"Token\"}, {\"data\": 115, \"type\": \"Token\"}"
                 ", {\"data\": 116, \"type\": \"Token\"}, {\"data\": 117, \"type\": \"Token\"}], \"N\": 5, \"tentative_read_pos\": {\"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\": 113}"
                 ", \"read_pos\": {\"98f7f8ad-f4d3-44ae-9228-7228a54c2fee3\": 113}}, \"id\": \"f4822772-bca4-465c-8109-a1e7a960e304\"}}"
                 ", \"id\": \"6bc4b467-a2d2-4254-ad84-bf918231c68e\"}}, \"cmd\": \"ACTOR_NEW\", \"msg_uuid\": \"fd71b46d-0e44-4298-9bb3-61aa0420bc45\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    // Test what number arduino should start counting from
    JsonObject &fifoObject = msg["state"]["actor_state"]["outports"]["integer"]["fifo"];
    JsonArray &fifoArray = fifoObject.get("fifo");
    JsonObject &count = fifoArray.get(0);
    EXPECT_EQ(113,(uint32_t)count.get("data"));

    int8_t size = mini.handleMsg(msg, reply, request, 0);
    JsonObject &actor_state = msg["state"]["actor_state"];

    // Test if number of outgoing messages is 2
    EXPECT_EQ(size,2);

    // Test if actor_type Counter is triggered
    EXPECT_STREQ("std.Counter", msg["state"]["actor_type"]);

    // Test if actor_state contains key count
    // which should be true when actor is Counter
    EXPECT_TRUE(actor_state.containsKey("count"));

    // Test if PORT_CONNECT is returned cmd
    // which it should be after an actor migrate
    EXPECT_STREQ("PORT_CONNECT", request["cmd"]);

    // Test code that searches for a '.' and
    // builds a string of the actor type name
    const char *p = msg["state"]["actor_type"].asString();
    String str2 = "";
    while(*p != '.')
    {
       str2 += *p;
       p++;
    }
    EXPECT_STREQ("std", str2.c_str());

    // Test code that compares part of actor type name
    uint8_t actorBool = !strncmp("std", msg["state"]["actor_type"].asString(), 3);
    EXPECT_EQ(1, actorBool);
}

TEST(testCounter, testCounterACK)
{
    CalvinMini mini;
    String str = "{\"to_rt_uuid\": \"calvin-arduino\", \"from_rt_uuid\": \"babc045f-5da1-4eb1-a08e-95e6a272cfde\""
                 ", \"cmd\": \"TUNNEL_DATA\", \"value\": {\"sequencenbr\": 0, \"peer_port_id\": \"50017184-4e7f-4f76-8c39-9e1f35f59b46\""
                 ", \"cmd\": \"TOKEN_REPLY\", \"port_id\": \"be1fbbb2-9fb3-4bac-bb94-0e601f2df6df\", \"value\": \"ACK\"}, \"tunnel_id\": \"fake-tunnel\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    int8_t size = mini.handleMsg(msg, reply, request, 0);

    // Test if number of outgoing messages is 1
    EXPECT_EQ(size, 1);

    JsonObject &value = reply.get("value");
    int sequencenbr = value.get("sequencenbr");

    // Test if sequencenbr is updated from 0 to 1 when ACK from base
    EXPECT_EQ(1, sequencenbr);
}

TEST(testCounter, testCounterNACK)
{
    CalvinMini mini;
    String str = "{\"to_rt_uuid\": \"calvin-arduino\", \"from_rt_uuid\": \"babc045f-5da1-4eb1-a08e-95e6a272cfde\""
                 ", \"cmd\": \"TUNNEL_DATA\", \"value\": {\"sequencenbr\": 0, \"peer_port_id\": \"50017184-4e7f-4f76-8c39-9e1f35f59b46\""
                 ", \"cmd\": \"TOKEN_REPLY\", \"port_id\": \"be1fbbb2-9fb3-4bac-bb94-0e601f2df6df\", \"value\": \"NACK\"}, \"tunnel_id\": \"fake-tunnel\"}";

    StaticJsonBuffer<4096> jsonBuffer;
    JsonObject &msg = jsonBuffer.parseObject(str.c_str());
    JsonObject &reply = jsonBuffer.createObject();
    JsonObject &request = jsonBuffer.createObject();

    int8_t size = mini.handleMsg(msg, reply, request, 0);

    // Test if number of outgoing messages is 1
    EXPECT_EQ(size, 1);

    JsonObject &value = reply.get("value");
    int sequencenbr = value.get("sequencenbr");

    // Test if sequencenbr is unchanged from 0 when NACK from base
    EXPECT_EQ(0, sequencenbr);
}

TEST(testCounter,testCounting)
{
	CalvinMini mini;
	actor testActor;
	fifo testFifo;
	initFifo(&testFifo);
	testActor.inportsFifo[0] = testFifo;
	fifoPop(&testActor.inportsFifo[0]);
	testActor.count = 0;
	EXPECT_EQ(0,testActor.count);
	EXPECT_EQ(0,actorCount(&testActor));
	EXPECT_EQ(1,testActor.count);
	fifoPop(&testActor.inportsFifo[0]);
	testActor.count = 0;
	for(int i = 0; i < 1000; i++){
		actorCount(&testActor);
		EXPECT_EQ(i+1,fifoPop(&testActor.inportsFifo[0]));
	}
	EXPECT_EQ(1000,testActor.count);

}

#endif
