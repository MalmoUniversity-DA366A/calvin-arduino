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
	testActor.type = "io.StandardOut";
	fifo testfifo;
	testActor.inportsFifo[0] = testfifo;
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

