/*
 * testHandleJoin.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testHandleJoin : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testHandleJoin, testReplyObject)
{
    CalvinMini mini;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject &msg = jsonBuffer.createObject();
    JsonObject &reply = jsonBuffer.createObject();
    mini.handleJoin(msg, reply, 0);

    // Test content of object
    EXPECT_TRUE(reply.containsKey("cmd"));
    EXPECT_TRUE(reply.containsKey("id"));
    EXPECT_TRUE(reply.containsKey("sid"));
    EXPECT_TRUE(reply.containsKey("serializer"));
    EXPECT_STREQ("JOIN_REPLY", reply.get("cmd"));
    EXPECT_STREQ("calvin-arduino", reply.get("id"));
    EXPECT_STREQ("json", reply.get("serializer"));
}

TEST(testHandleJoin, testSize)
{
    CalvinMini mini;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject &msg = jsonBuffer.createObject();
    JsonObject &reply = jsonBuffer.createObject();
    mini.handleJoin(msg, reply, 0);

    // Test size of object
    EXPECT_EQ(4, reply.size());
}
#endif
