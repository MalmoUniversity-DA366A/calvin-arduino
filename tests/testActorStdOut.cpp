/*
 * testActorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "../src/actorStdOut.h"
#include "gtest/gtest.h"
#include "ArduinoJson.h"

class ActorTest : public ::testing::Test {
	protected:
	 virtual void SetUp(){
	 }

	 virtual void TearDown(){
	 }
};

TEST(ActorTest,jsonTest){
	ActorStdOut actorstd;
	EXPECT_EQ(1,actorstd.createJson());
}
/**
 * Testing the actor struct.
 */
TEST(ActorTest,structTest){
	ActorStdOut actorstd;
	actorstd.createJson();
	actor newActor;
	actor globalActor;
	globalActor = actorstd.getGlobalStruct();
	newActor.type = "Daniel";
	newActor.name = "Hej";
	newActor.id = "1";
	newActor.outport = "7411";
	newActor.inport = "1337";
	EXPECT_EQ("Daniel",newActor.type);
	EXPECT_EQ("Hej",newActor.name);
	EXPECT_EQ("1",newActor.id);
	EXPECT_EQ("7411",newActor.outport);
	EXPECT_EQ("1337",newActor.inport);

	EXPECT_STREQ("actor",globalActor.type);
	EXPECT_STREQ("actor1",globalActor.name);
	EXPECT_STREQ("actor2",globalActor.id);
	EXPECT_STREQ("Test me",globalActor.fifo);
	EXPECT_STREQ("12",globalActor.outport);



}

TEST(ActorTest,createActor){
	ActorStdOut actorstd;
	//EXPECT_EQ(1,actorstd.createActor());
}

TEST(ActorTest,actorStdOut){
	ActorStdOut actorstd;
	actor globalActor;
	globalActor = actorstd.getGlobalStruct();
	int testValue = actorstd.StdOut(globalActor);
	EXPECT_EQ(3,testValue);
}







