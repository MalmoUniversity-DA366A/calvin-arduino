/*
 * testNackFromCalvinBase.cpp
 *
 *  Created on: 15 okt. 2015
 *      Author: Jesper Hansen
 */

/*
#ifdef _MOCK_
#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testNackFromCalvinBase : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testSendToken, testIfContainsKey)
{
	CalvinMini calvinMini;
	actorInit();

	StaticJsonBuffer<4000> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &token = jsonBuffer.createObject();

	token["id"] = "1234";
	//msg["state"]["actor_state"]["inports"]["token"] = token;

	calvinMini.sendToken(msg, reply, request, token);

	EXPECT_TRUE(reply.containsKey("cmd"));
	EXPECT_STREQ(request.get("type"), "TOKEN");
}
#endif
*/

