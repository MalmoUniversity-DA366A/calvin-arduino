/*
 * testHandleSetupPorts.cpp
 *
 *  Created on: 7 okt. 2015
 *      Author: Daniel Nordahl
 */

#include "../src/CalvinDone/CalvinMini.h"
#include "gtest/gtest.h"
#include "ArduinoJson.h"

class SetupPortTest : public ::testing::Test {
  protected:
   virtual void SetUp(){
   }

   virtual void TearDown(){
   }
};

/**
 * Test handleport function
 */
TEST(SetupPortTest,HandlePorts)
{
	CalvinMini mini;
	actorInit();
	// Create two empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<400> jsonBuffer;
	JsonObject &reply = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &request = jsonBuffer.createObject();
	mini.handleSetupPorts(msg,reply,request);

	EXPECT_STREQ(request.get("msg_uuid"),
			"MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb7");
}






