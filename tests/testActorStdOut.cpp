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

	//Test struct with json string
	EXPECT_STREQ("actor",globalActor.type);
	EXPECT_STREQ("actor1",globalActor.name);
	EXPECT_STREQ("89",globalActor.id);
	EXPECT_STREQ("12",globalActor.fifo);
	EXPECT_STREQ("NULL",globalActor.outport);


}

TEST(ActorTest,createActor){
	ActorStdOut actorstd;
	StaticJsonBuffer<2000> jsonBuffer;
	char str[] = "{\"type\":\"actor\",\"name\":\"actor1\","
			"\"id\":\"89\",\"fifo\":\"12\"}";
	JsonObject &root = jsonBuffer.parseObject(str);
	//EXPECT_EQ(1,actorstd.createActor(root));
}

TEST(ActorTest,actorFire){
	ActorStdOut actorstd;
	actor globalActor;
	globalActor = actorstd.getGlobalStruct();
	EXPECT_EQ(2,globalActor.function());
}

TEST(ActorTest,processTest){
	ActorStdOut actorstd;
	actorInit();
	EXPECT_EQ(0,actorstd.process("Test1"));
	EXPECT_EQ(0,actorstd.process("Test2"));
	EXPECT_EQ(0,actorstd.process("Test3"));

}


/**
 * I need to figure out how to make the actorstruct
 * global before these tests can be runned.
 */
TEST(ActorTest,testKeys){
	/*
	ActorStdOut actorstd;
	int8_t* array;
	array = actorstd.searchForKeys("ett","sex","sju");
	EXPECT_EQ(4,array[0]);
	EXPECT_EQ(0,array[1]);
	EXPECT_EQ(3,array[2]);

	main
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
	*/
}
/**
 * Test the fifo
 */
TEST(ActorTest,TestFifo){
	fifo actorFifo;
	EXPECT_EQ(0,initFifo(&actorFifo));
	EXPECT_EQ(0,fifoAdd(&actorFifo,"Hej"));
	EXPECT_STREQ("Hej",fifoPop(&actorFifo));
	EXPECT_EQ(0,fifoAdd(&actorFifo,"Daniel"));
	EXPECT_EQ(0,fifoAdd(&actorFifo,"Hanna"));
	EXPECT_EQ(0,fifoAdd(&actorFifo,"Ebbe"));
	EXPECT_STREQ("Daniel",fifoPop(&actorFifo));
	EXPECT_STREQ("Hanna",fifoPop(&actorFifo));
	EXPECT_STREQ("Ebbe",fifoPop(&actorFifo));


	for( int i = 0 ; i < 7 ; i++ ){
		EXPECT_EQ(0,fifoAdd(&actorFifo,"Ebbe"));
	}
	/*If fifo size is 8 this is where it fills upp size-1*/
	EXPECT_EQ(-1,fifoAdd(&actorFifo,"Ebbe"));

	/*Now we pop it*/
	for( int i = 0 ; i < 7 ; i++ ){
		EXPECT_STREQ("Ebbe",fifoPop(&actorFifo));
	}
	EXPECT_STREQ("Null",fifoPop(&actorFifo));;

}









