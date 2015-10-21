/*
 * testActorList.cpp
 *
 *  Created on: 21 okt. 2015
 *      Author: Daniel
 */

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testActorList : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testActorList,listTest){
	const char* str1 = "io.StandardOut";
	const char* str2 = "std.Counter";
	const char* str3 = "failme";
	actor testActor1;
	actor testActor2;
	actor testList[NUMBER_OF_SUPPORTED_ACTORS];
	CalvinMini *mini = new CalvinMini;
	testActor1.type = "io.StandardOut";
	testActor2.type = "std.Counter";
	testList[0] = testActor1;
	testList[1] = testActor2;
	EXPECT_EQ(0,mini->getActorPos(str1,testList));
	EXPECT_EQ(1,mini->getActorPos(str2,testList));
	EXPECT_EQ(255,mini->getActorPos(str3,testList));


	testActor1.type = "std.Counter";
	testActor2.type = "io.StandardOut";
	EXPECT_EQ(0,mini->getActorPos(str1,testList));
	EXPECT_EQ(1,mini->getActorPos(str2,testList));
}
