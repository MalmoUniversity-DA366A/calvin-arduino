/*
 * testHandleActor.cpp
 *
 *  Created on: 7 okt. 2015
 *      Author: Daniel Nordahl
 */

#include "../src/CalvinDone/CalvinMini.h"
#include "gtest/gtest.h"
#include "ArduinoJson.h"

class HandleTestActor : public ::testing::Test {
	protected:
	 virtual void SetUp(){
	 }

	 virtual void TearDown(){
	 }
};

TEST(HandleTest,TestCreateActor){
	const char* test;
	CalvinMini *mini = new CalvinMini;
	StaticJsonBuffer<4000> jsonBuffer;
	JsonObject &msg = jsonBuffer.createObject();

	StaticJsonBuffer<4000> jsonBuffer1;
	JsonObject &state = jsonBuffer.createObject();

	StaticJsonBuffer<4000> jsonBuffer2;
	JsonObject &name = jsonBuffer.createObject();

	state["actor_type"] = "io.StandardOut";
	name.set("id","1337");
	name.set("name","Jsper");
	state.set("name",name);
	msg.set("state", state);
	mini->createActor(msg,0);


	EXPECT_STREQ("Jsper",name.get("name"));
	JsonObject &testState = msg.get("state");
	JsonObject &name1 = state.get("name");
	test = testState.get("actor_type");
	EXPECT_STREQ("io.StandardOut",test);
	test = name1.get("id");
	EXPECT_STREQ("1337",test);
	delete mini;

}


