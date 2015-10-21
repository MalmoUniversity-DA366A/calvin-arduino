/*
 * test_HandleToken.cpp
 *
 *  Created on: 30 sep. 2015
 *      Author: Jesper Hansen
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinInProgress/Token.h"
#include "../src/CalvinDone/CalvinMini.h"

class test_HandleToken : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(test_HandleToken, testProcess) {
	actor testActor;
	fifo testFifo;
	// Create an instance of the Token
	CalvinMini mini;
	// Create two empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &token = jsonBuffer.createObject();

	// Add String values for testing, remove in the final project.
	msg["sequencenbr"] = "1234";
	msg["port_id"] = "4321";
	msg["peer_port_id"] = "5678";
	token.set("data",6666);
	msg.set("token",token);
	initFifo(&testFifo);
	testActor.inportsFifo[0] = testFifo;
	fifoAdd(&testActor.inportsFifo[0],44444);
	// Initiate the handleToken function so that JsonObject reply
	// is filled.
	mini.handleToken(msg, reply);
	EXPECT_STREQ("ACK", reply.get("value"));

	EXPECT_EQ(5, actorStdOut(&testActor));
}

TEST(test_HandleToken, testSize) {
	// Create an instance of the Token
	CalvinMini token;
	// Create two empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();

	// Add String values for testing, remove in the final project.
	msg["sequencenbr"] = "1234";
	msg["port_id"] = "4321";
	msg["peer_port_id"] = "5678";
	msg["token"] = "6666";

	// Initiate the handleToken function so that JsonObject reply
	// is filled.
	token.handleToken(msg, reply);

	EXPECT_EQ(5, reply.size());
}

TEST(test_HandleToken, testIfContainsKey) {
	// Create an instance of the Token
	CalvinMini token;
	// Create two empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();

	// Add String values for testing, remove in the final project.
	msg["sequencenbr"] = "1234";
	msg["port_id"] = "4321";
	msg["peer_port_id"] = "5678";
	msg["token"] = "6666";

	// Initiate the handleToken function so that JsonObject reply
	// is filled.
	token.handleToken(msg, reply);

	// Checks if reply contains the key that we want
	EXPECT_TRUE(reply.containsKey("cmd"));
	EXPECT_TRUE(reply.containsKey("sequencenbr"));
	EXPECT_TRUE(reply.containsKey("port_id"));
	EXPECT_TRUE(reply.containsKey("peer_port_id"));
	EXPECT_TRUE(reply.containsKey("value"));
}

TEST(test_HandleToken, testValues) {
	// Create an instance of the Token
	CalvinMini token;
	// Create two empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();

	// Add String values for testing, remove in the final project.
	msg["sequencenbr"] = "1234";
	msg["port_id"] = "4321";
	msg["peer_port_id"] = "5678";
	msg["token"] = "6666";

	// Initiate the handleToken function so that JsonObject reply
	// is filled.
	token.handleToken(msg, reply);

	// Checks if the reply value is correct
	EXPECT_STREQ("TOKEN_REPLY", reply.get("cmd"));
	EXPECT_STREQ(msg.get("sequencenbr"), reply.get("sequencenbr"));
	EXPECT_STREQ("4321", reply.get("port_id"));
	EXPECT_STREQ("5678", reply.get("peer_port_id"));
	EXPECT_STREQ("ACK", reply.get("value"));
}
#endif
