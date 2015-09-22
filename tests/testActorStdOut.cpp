/*
 * testActorStdOut.cpp
 *
 *  Created on: 22 sep. 2015
 *      Author: Daniel Nordahl
 */

#include "../src/actorStdOut.h"
#include "gtest/gtest.h"

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
TEST(ActorTest,NULL){
	ActorStdOut actorstd;
	actor newActor;
	newActor.name = 'D';
	newActor.id = '1';
	newActor.outport = '3';
	newActor.inport = '4';
	EXPECT_EQ('D',newActor.name);
	EXPECT_EQ('1',newActor.id);
	EXPECT_EQ('3',newActor.outport);
	EXPECT_EQ('4',newActor.inport);
}







