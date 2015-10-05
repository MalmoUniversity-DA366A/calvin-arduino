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

/*TEST(ActorTest,jsonTest)
{
	CavinMini actorstd;
	EXPECT_EQ(1,actorstd.createJson());
}*/
/**
 * Testing the actor struct.
 */

TEST(ActorTest,structTest)
{
	CalvinMini actorstd;
	actor newActor;

	//Test struct
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
 * Testing actor key search function
 */
TEST(ActorTest,testKeys)
{
	actor globalActor;
	CalvinMini actorstd;
	int8_t* array;

	globalActor.value[0].key = "e4tt";
	globalActor.value[1].key = "e22tt";
	globalActor.value[2].key = "etdt";
	globalActor.value[3].key = "esstft";
	globalActor.value[4].key = "ett";
	globalActor.value[4].value[0].key = "sex";
	globalActor.value[4].value[1].key = "se2x";
	globalActor.value[4].value[2].key = "se2x";
	globalActor.value[4].value[3].key = "s2ex";
	globalActor.value[4].value[4].key = "se2x";

	globalActor.value[4].value[0].value[0].key = "sex";
	globalActor.value[4].value[0].value[1].key  = "se2x";
	globalActor.value[4].value[0].value[2].key = "se2x";
	globalActor.value[4].value[0].value[3].key  = "sju";
	globalActor.value[4].value[0].value[4].key  = "se2x";

	array = actorstd.searchForKeys(&globalActor,"ett","sex","sju");
	EXPECT_EQ(4,array[0]);
	EXPECT_EQ(0,array[1]);
	EXPECT_EQ(3,array[2]);
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









