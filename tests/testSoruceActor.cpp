/*
 * testSoruceActor.cpp
 *
 *  Created on: 13 okt. 2015
 *      Author: Daniel
 */

#include "gtest/gtest.h"
#include "../src/CalvinInProgress/SourceActor.h"

class testSourceActor : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testSoruceActor,testSetup)
{
	SourceActor mini;
	EXPECT_EQ(0,mini.actorInit());
}

TEST(testSourceActor,testIntFifo){
	SourceActor mini;
	intFifo testFifo;
	mini.initFifo(&testFifo);
	EXPECT_EQ(SUCCESS,mini.fifoAdd(&testFifo,1337));
	EXPECT_EQ(1337,mini.fifoPop(&testFifo));

	EXPECT_EQ(FAIL,mini.fifoPop(&testFifo));

	for(int i = 0; i < 7; i++){
		EXPECT_EQ(SUCCESS,mini.fifoAdd(&testFifo,i));
	}
	EXPECT_EQ(FAIL,mini.fifoAdd(&testFifo,1337));

	for(int i = 0; i < 7; i++){
			EXPECT_EQ(i,mini.fifoPop(&testFifo));
	}

}

