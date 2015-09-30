/*
 * test_HandleToken.cpp
 *
 *  Created on: 30 sep. 2015
 *      Author: Jesper Hansen
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/Token.h"

class test_handleToken : public ::testing:Test {
protected:
	virtual void SetUp() {}
	vitrual void TearDown() {}
};

TEST(test_HandleToken, handleToken) {
	Token token;

	StaticJsonBuffer<300> buffer;
	JsonObject &reply = buffer.createObject();


}

#endif
