/*
 * test_setupTunnel.cpp
 *
 *  Created on: 24 sep. 2015
 *      Author: Jesper
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/setupTunnel.h"

class test_setupTunnel : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(test_setupTunnel, handleSetupTunnel) {
	setupTunnel st;
	StaticJsonBuffer<1000> buffer;
	JsonObject &reply = buffer.createObject();

	StaticJsonBuffer<200> jsonBuffer;
	char json[] = "{\"sensor\":\"gps\",\"msg_uuid\":1351824120,\"data\":[48.756080,2.302038]}";
	JsonObject &root = jsonBuffer.parseObject(json);

	EXPECT_EQ(1, st.handleSetupTunnel(root ,reply));

}
#endif
