/*
 * test_setupTunnel.cpp
 *
 *  Created on: 24 sep. 2015
 *      Author: Jesper Hansen
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/setupTunnel.h"

class test_setupTunnel : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

// A test method for testing the method handleSetupTunnel()
TEST(test_setupTunnel, handleSetupTunnel) {
	// Create an instance variable of the setupTunnel class
	setupTunnel st;

	// Create an empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<300> buffer;
	JsonObject &request = buffer.createObject();

	// Create an dummy JsonObject with an random value nbr in key msg_uuid,
	// then send the JsonObject as a parameter
	StaticJsonBuffer<100> jsonBuffer;
	char json[] = "{\"msg_uuid\":\"123456789\"}";
	JsonObject &msg = jsonBuffer.parseObject(json);

	// Test for initiate the method so that the empty JsonObject is being filled,
	// if the object is valid it will return a 1. Otherwise 2.
	EXPECT_EQ(1, st.handleSetupTunnel(msg ,request));

	// Test for checking the size of the JsonObject
	EXPECT_EQ(7, request.size());

	// Test for checking if the string value in the JsonObject is equal to the set string
	EXPECT_STREQ("MSG-12345678-1234-5678-1234-567812345678", request.get("msg_uuid"));
	EXPECT_STREQ("calvin-miniscule", request.get("from_rt_uuid"));

	EXPECT_STREQ(msg["msg_uuid"], request.get("to_rt_uuid"));


	EXPECT_STREQ("TUNNEL_NEW", request.get("cmd"));
	EXPECT_STREQ("fake-tunnel", request.get("tunnel_id"));
	EXPECT_STREQ("token", request.get("type"));
	EXPECT_TRUE(request.containsKey("policy"));
	EXPECT_EQ(0, request.get("policy").size());
}

// A test for the handleTunnelData
TEST(test_setupTunnel, handleTunnelData) {
	// Create an instance variable of the setupTunnel class
	setupTunnel st;

	// Create an empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<200> buffer;
	JsonObject &reply = buffer.createObject();

	// Create an dummy JsonObject with an random value nbr in key msg_uuid,
	// then send the JsonObject as a parameter
	StaticJsonBuffer<100> jsonBuffer;
	char json[] = "{\"msg_uuid\":\"123456789\"}";
	JsonObject &msg = jsonBuffer.parseObject(json);

	// Test for initiate the method so that the empty JsonObject is being filled,
	// if the object is valid it will return a 1. Otherwise 2.
	EXPECT_EQ(1, st.handleTunnelData(msg ,reply));

	// Test for checking the size of the JsonObject
	EXPECT_EQ(5, reply.size());


	// Test for checking if the string value in the JsonObject is equal to the set string

	// This test isn't fulfilled because I don't think the dummy JsonObject isn't working correctl
	EXPECT_STREQ("1234", reply.get("to_rt_uuid"));
	EXPECT_STREQ("1234", reply.get("from_rt_uuid"));

	EXPECT_STREQ("TUNNEL_DATA", reply.get("cmd"));
	EXPECT_STREQ("fake-tunnel", reply.get("tunnel_id"));
	EXPECT_STREQ("foo", reply.get("value"));

}
#endif
