/*
 * test_handleMessage.cpp
 *
 *  Created on: 27 sep. 2015
 *      Author: Jesper Hansen
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinInProgress/handleMessage.h"

class test_handleMessage : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(test_handleMessage, handleMsg) {
	MessageHandler messageHandler;

	// Create an JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &msg = jsonBuffer.createObject();

	// Test JOIN_REQUEST in handleMsg
	msg["cmd"] = "JOIN_REQUEST";
	EXPECT_EQ(1, messageHandler.handleMsg(msg));

	// Test ACTOR_NEW in handleMsg
	msg["cmd"] = "ACTOR_NEW";
	EXPECT_EQ(2, messageHandler.handleMsg(msg));

	// Test TUNNEL_DATA in handleMsg
	msg["cmd"] = "TUNNEL_DATA";
	EXPECT_EQ(3, messageHandler.handleMsg(msg));

	// Test TOKEN in handleMsg
	msg["cmd"] = "TOKEN";
	EXPECT_EQ(4, messageHandler.handleMsg(msg));

	// Test TOKEN_REPLY in handleMsg
	msg["cmd"] = "TOKEN_REPLY";
	EXPECT_EQ(5, messageHandler.handleMsg(msg));

	// Test REPLY in handleMsg
	msg["cmd"] = "REPLY";
	EXPECT_EQ(6, messageHandler.handleMsg(msg));

}

#endif
