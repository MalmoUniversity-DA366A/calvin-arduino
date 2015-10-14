/*
 * testSoruceActor.cpp
 *
 *  Created on: 13 okt. 2015
 *      Author: Daniel
 */

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testSourceActor : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testSoruceActor,testSetup)
{
	CalvinMini mini;
	actor testActor;
	fifo testfifo;
	testActor.inportsFifo[0] = &testfifo;
	EXPECT_EQ(0,actorInit(&testActor));
}

TEST(testSourceActor,testIntFifo){
	CalvinMini mini;
	fifo testFifo;
	initFifo(&testFifo);
	EXPECT_EQ(SUCCESS,fifoAdd(&testFifo,1337));
	EXPECT_EQ(1337,fifoPop(&testFifo));

	EXPECT_EQ(FAIL,fifoPop(&testFifo));

	for(int i = 0; i < 7; i++){
		EXPECT_EQ(SUCCESS,fifoAdd(&testFifo,i));
	}
	EXPECT_EQ(FAIL,fifoAdd(&testFifo,1337));

	for(int i = 0; i < 7; i++){
			EXPECT_EQ(i,fifoPop(&testFifo));
	}

}

TEST(testSourceActor,testActorCounter){
	CalvinMini mini;
	actor source;
	fifo sourceFifo;

	source.count = 0;
	EXPECT_EQ(0,source.count);

	source.inportsFifo[0] = &sourceFifo;
	initFifo(&sourceFifo);
	EXPECT_EQ(SUCCESS,actorCount(&source));
	EXPECT_EQ(1,fifoPop(source.inportsFifo[0]));

	source.count = 0;
	EXPECT_EQ(0,source.count);

	for(int i = 0; i <= 100; i++){
		EXPECT_EQ(SUCCESS,actorCount(&source));
		EXPECT_EQ((i+1),fifoPop(source.inportsFifo[0]));
	}


}

