/*
 * This is an example test to show how GoogleTest
 * work.
 *  Created on: 21 sep. 2015
 *      Author: Daniel
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/Addition.hpp"

class AdditionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST(AdditionTest,twoValues){
    const int x = 4;
    const int y = 5;
    Addition addition;
    EXPECT_EQ(9,addition.twoValues(x,y));
    EXPECT_EQ(42,addition.twoValues(21,21));
}

TEST(AdditonTest,twoValues){
	Addition addition;
	EXPECT_EQ(2,addition.twoValues(1,1));
}

#endif
