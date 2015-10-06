/*
 * testActorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "../src/CalvinDone/CalvinMini.h"
#include "gtest/gtest.h"
#include "ArduinoJson.h"

class ActorTest : public ::testing::Test {
	protected:
	 virtual void SetUp(){
	 }

	 virtual void TearDown(){
	 }
};

/**
 * Testing the actor struct.
 */

TEST(ActorTest,structTest)
{
	CalvinMini actorstd;
	actor newActor;
	fifo actorFifo;
	EXPECT_EQ(0,initFifo(&actorFifo));
	//Test struct
	newActor.type = "Daniel";
	newActor.name = "Hej";
	newActor.id = "1";

	/*Test inport fifo*/
	newActor.inportsFifo[0] = &actorFifo;
	EXPECT_EQ(0,fifoAdd(newActor.inportsFifo[0],"Add"));
	EXPECT_STREQ("Add",fifoPop(newActor.inportsFifo[0]));
	/*Test outportfifo*/
	newActor.outportsFifo[0] = &actorFifo;
	EXPECT_EQ(0,fifoAdd(newActor.inportsFifo[0],"Add"));
	EXPECT_STREQ("Add",fifoPop(newActor.inportsFifo[0]));

	EXPECT_EQ("Daniel",newActor.type);
	EXPECT_EQ("Hej",newActor.name);
	EXPECT_EQ("1",newActor.id);

}

TEST(ActorTest,Struct){

	actor newTest;
	fifo actorFifo;
	EXPECT_EQ(0,initFifo(&actorFifo));
	newTest.inportsFifo[0] = &actorFifo;
	EXPECT_EQ(0,fifoAdd(newTest.inportsFifo[0],"Calvin"));
	EXPECT_STREQ("Calvin",fifoPop(newTest.inportsFifo[0]));

}

TEST(ActorTest,createActor)
{
	CalvinMini actorstd;
	StaticJsonBuffer<2000> jsonBuffer;
	char str[] = "{\"type\":\"actor\",\"name\":\"actor1\","
			"\"id\":\"89\",\"fifo\":\"12\"}";
	JsonObject &root = jsonBuffer.parseObject(str);
	EXPECT_EQ(1,actorstd.createActor(root));
}


TEST(ActorTest,processTest)
{
	CalvinMini actorstd;
	actorInit();
	EXPECT_EQ(0,actorstd.process("Test1"));
	EXPECT_EQ(0,actorstd.process("Test2"));
	EXPECT_EQ(0,actorstd.process("Test3"));
	EXPECT_EQ(0,actorstd.process("Test1"));
	EXPECT_EQ(0,actorstd.process("Test2"));
	EXPECT_EQ(0,actorstd.process("Test3"));
	EXPECT_EQ(0,actorstd.process("Test2"));
	EXPECT_EQ(-1,actorstd.process("Test3"));

}

/**
 * Test the fifo
 */
TEST(ActorTest,TestFifo)
{
	fifo actorFifo;
	const char* test1 = "calvin";
	const char* test2 = "base";
	const char* test3 = "base1";
	const char* test4 = "base2";
	EXPECT_EQ(0,initFifo(&actorFifo));
	EXPECT_EQ(0,fifoAdd(&actorFifo,test1));
	EXPECT_STREQ(test1,fifoPop(&actorFifo));
	EXPECT_EQ(0,fifoAdd(&actorFifo,test1));
	EXPECT_EQ(0,fifoAdd(&actorFifo,test2));
	EXPECT_EQ(0,fifoAdd(&actorFifo,test3));
	EXPECT_STREQ(test1,fifoPop(&actorFifo));
	EXPECT_STREQ(test2,fifoPop(&actorFifo));
	EXPECT_STREQ(test3,fifoPop(&actorFifo));


	for( int i = 0 ; i < 7 ; i++ ){
		EXPECT_EQ(0,fifoAdd(&actorFifo,test1));
	}
	/*If fifo size is 8 this is where it fills upp size-1*/
	EXPECT_EQ(-1,fifoAdd(&actorFifo,test1));

	/*Now we pop it*/
	for( int i = 0 ; i < 7 ; i++ ){
		EXPECT_STREQ(test1,fifoPop(&actorFifo));
	}
	/*It should be full by now*/
	EXPECT_STREQ("Null",fifoPop(&actorFifo));;

}









