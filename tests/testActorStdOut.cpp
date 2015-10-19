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
  actor newActor;
  fifo actorFifo;
  EXPECT_EQ(0,initFifo(&actorFifo));
  //Test struct
  newActor.type = "Daniel";
  newActor.name = "Hej";
  newActor.id = "1";

  /*Test inport fifo*/
  newActor.inportsFifo[0] = &actorFifo;
  EXPECT_EQ(0,fifoAdd(newActor.inportsFifo[0],555));
  EXPECT_EQ(555,fifoPop(newActor.inportsFifo[0]));
  /*Test outportfifo*/
  newActor.outportsFifo[0] = &actorFifo;
  EXPECT_EQ(0,fifoAdd(newActor.inportsFifo[0],666));
  EXPECT_EQ(666,fifoPop(newActor.inportsFifo[0]));

  EXPECT_EQ("Daniel",newActor.type);
  EXPECT_EQ("Hej",newActor.name);
  EXPECT_EQ("1",newActor.id);

}

TEST(ActorTest,Struct){

  actor newTest;
  fifo actorFifo;
  EXPECT_EQ(SUCCESS,initFifo(&actorFifo));
  newTest.inportsFifo[0] = &actorFifo;
  EXPECT_EQ(SUCCESS,fifoAdd(newTest.inportsFifo[0],1337));
  EXPECT_EQ(1337,fifoPop(newTest.inportsFifo[0]));

}

TEST(ActorTest,createActor)
{
  CalvinMini *actorstd = new CalvinMini;
  StaticJsonBuffer<4096> jsonBuffer;
  String str2 = "{\"to_rt_uuid\": \"calvin-miniscule\", \"from_rt_uuid\": \"422828ae-02c1-4d2f-9837-43a32fb6decb\""
                   ", \"state\": {\"prev_connections\": {\"actor_name\": \"test3:src\", \"inports\": {}, \"actor_id\": \"6bc4b467-a2d2-4254-ad84-bf918231c68e\""
                   ", \"outports\": {\"f4822772-bca4-465c-8109-a1e7a960e304\": [[\"422828ae-02c1-4d2f-9837-43a32fb6decb\", \"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\"]]}}"
                   ", \"actor_type\": \"std.Counter\", \"actor_state\": {\"count\": 117, \"name\": \"test3:src\", \"inports\": {}, \"_managed\": [\"count\", \"id\", \"name\"]"
                   ", \"outports\": {\"integer\": {\"fanout\": 1, \"name\": \"integer\", \"fifo\": {\"write_pos\": 117, \"readers\": [\"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\"]"
                   ", \"fifo\": [{\"data\": 116, \"type\": \"Token\"}, {\"data\": 117, \"type\": \"Token\"}, {\"data\": 113, \"type\": \"Token\"}"
                   ", {\"data\": 114, \"type\": \"Token\"}, {\"data\": 115, \"type\": \"Token\"}], \"N\": 5, \"tentative_read_pos\": {\"98f7f8ad-f4d3-44ae-9228-728a54c2fee3\": 113}"
                   ", \"read_pos\": {\"98f7f8ad-f4d3-44ae-9228-7228a54c2fee3\": 113}}, \"id\": \"f4822772-bca4-465c-8109-a1e7a960e304\"}}"
                   ", \"id\": \"6bc4b467-a2d2-4254-ad84-bf918231c68e\"}}, \"cmd\": \"AdCTOR\", \"msg_uuid\": \"fd71b46d-0e44-4298-9bb3-61aa0420bc45\"}";

  JsonObject &root = jsonBuffer.parseObject(str2.c_str());
  EXPECT_EQ(SUCCESS,actorstd->createActor(root));
  delete actorstd;
}


TEST(ActorTest,processTest)
{
  CalvinMini actorstd;
  EXPECT_EQ(SUCCESS,actorInitTest());
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(SUCCESS,actorstd.process(1337));
  EXPECT_EQ(FAIL,actorstd.process(1337));

}

/**
 * Test the fifo
 */
TEST(ActorTest,TestFifo)
{
  fifo actorFifo;
  uint32_t test1 = 1;
  uint32_t test2 = 2;
  uint32_t test3 = 3;
  EXPECT_EQ(SUCCESS,initFifo(&actorFifo));
  EXPECT_EQ(SUCCESS,fifoAdd(&actorFifo,test1));
  EXPECT_EQ(test1,fifoPop(&actorFifo));
  EXPECT_EQ(SUCCESS,fifoAdd(&actorFifo,test1));
  EXPECT_EQ(SUCCESS,fifoAdd(&actorFifo,test2));
  EXPECT_EQ(SUCCESS,fifoAdd(&actorFifo,test3));
  EXPECT_EQ(test1,fifoPop(&actorFifo));
  EXPECT_EQ(test2,fifoPop(&actorFifo));
  EXPECT_EQ(test3,fifoPop(&actorFifo));


  for( int i = 0 ; i < 7 ; i++ ){
    EXPECT_EQ(SUCCESS,fifoAdd(&actorFifo,test1));
  }
  /*If fifo size is 8 this is where it fills upp size-1*/
  EXPECT_EQ(FAIL,fifoAdd(&actorFifo,test1));

  /*Now we pop it*/
  for( int i = 0 ; i < 7 ; i++ ){
    EXPECT_EQ(test1,fifoPop(&actorFifo));
  }
  /*It should be full by now*/
  EXPECT_EQ(FAIL,fifoPop(&actorFifo));;

}
