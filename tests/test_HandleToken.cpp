/*
 * test_HandleToken.cpp
 *
 *  Created on: 30 sep. 2015
 *      Author: Jesper Hansen
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/Token.h"

class test_HandleToken : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

// A test method for testing the handleToken function
TEST(test_HandleToken, handleToken) {
	// Create an instance of the Token
	Token token;

	// Create two empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();

	// Add String values for testing, remove in the final project.
	msg["sequencenbr"] = "1234";
	msg["port_id"] = "4321";
	msg["peer_port_id"] = "5678";

	// Initiate the handleToken function so that JsonObject reply
	// is filled. If object is valid it will return 1, otherwise 2
	EXPECT_EQ(1, token.handleToken(msg, reply));

	// Checks if reply contains the key that we want
	EXPECT_TRUE(reply.containsKey("cmd"));
	EXPECT_TRUE(reply.containsKey("sequencenbr"));
	EXPECT_TRUE(reply.containsKey("port_id"));
	EXPECT_TRUE(reply.containsKey("peer_port_id"));
	EXPECT_TRUE(reply.containsKey("value"));

	// Checks if the JsonObject is the correct size
	EXPECT_EQ(5, reply.size());

	// Checks if the reply value is correct
	EXPECT_STREQ("TOKEN_REPLY", reply.get("cmd"));
	EXPECT_STREQ(msg.get("sequencenbr"), reply.get("sequencenbr"));
	EXPECT_STREQ("4321", reply.get("port_id"));
	EXPECT_STREQ("5678", reply.get("peer_port_id"));
	EXPECT_STREQ("ACK", reply.get("value"));
}

#endif
