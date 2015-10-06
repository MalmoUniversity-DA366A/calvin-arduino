
/*
 * test_setupTunnel.cpp
 *
 *  Created on: 24 sep. 2015
 *      Author: Jesper Hansen
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"

class testHandleSetupTunnel : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

// A test method for testing the method handleSetupTunnel()
TEST(testHandleSetupTunnel, testSetStringInMethod) {
	// Create an instance variable of the setupTunnel class
	CalvinMini calvinMini;

	// Create an empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &policy = jsonBuffer.createObject();

	// Add a string value to msg_uuid key
	msg["id"] = "123456789";

	// Test for initiate the method so that the empty JsonObject is being filled,
	// if the object is valid it will return a 1. Otherwise 2.
	calvinMini.handleSetupTunnel(msg ,request, policy);

	// Test for checking if the string value in the JsonObject is equal to the set string
	EXPECT_STREQ("MSG-12345678-9101-1123-1415-161718192021", request.get("msg_uuid"));
	EXPECT_STREQ("calvin-miniscule", request.get("from_rt_uuid"));
	EXPECT_STREQ(msg.get("id"), request.get("to_rt_uuid"));
	EXPECT_STREQ("TUNNEL_NEW", request.get("cmd"));
	EXPECT_STREQ("fake-tunnel", request.get("tunnel_id"));
	EXPECT_STREQ("token", request.get("type"));
}

TEST(testHandleSetupTunnel, testSize) {
	// Create an instance variable of the setupTunnel class
	CalvinMini calvinMini;

	// Create an empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &policy = jsonBuffer.createObject();

	// Add a string value to msg_uuid key
	msg["id"] = "123456789";

	// Test for initiate the method so that the empty JsonObject is being filled,
	// if the object is valid it will return a 1. Otherwise 2.
	calvinMini.handleSetupTunnel(msg ,request, policy);

	// Test for checking the size of the JsonObject
	EXPECT_EQ(7, request.size());
}

TEST(testHandleSetupTunnel, testContainsKey) {
	// Create an instance variable of the setupTunnel class
	CalvinMini calvinMini;

	// Create an empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &policy = jsonBuffer.createObject();

	// Add a string value to msg_uuid key
	msg["id"] = "123456789";

	// Test for initiate the method so that the empty JsonObject is being filled,
	// if the object is valid it will return a 1. Otherwise 2.
	calvinMini.handleSetupTunnel(msg ,request, policy);

	// Checks if JsonObject contains the following keys
	EXPECT_TRUE(request.containsKey("msg_uuid"));
	EXPECT_TRUE(request.containsKey("from_rt_uuid"));
	EXPECT_TRUE(request.containsKey("to_rt_uuid"));
	EXPECT_TRUE(request.containsKey("cmd"));
	EXPECT_TRUE(request.containsKey("tunnel_id"));
	EXPECT_TRUE(request.containsKey("type"));
	EXPECT_TRUE(request.containsKey("policy"));
}

TEST(testHandleSetupTunnel, testPolicySize) {
	// Create an instance variable of the setupTunnel class
	CalvinMini calvinMini;

	// Create an empty JsonObject with an StaticJsonBuffer
	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject &request = jsonBuffer.createObject();
	JsonObject &msg = jsonBuffer.createObject();
	JsonObject &policy = jsonBuffer.createObject();

	// Add a string value to msg_uuid key
	msg["id"] = "123456789";

	// Test for initiate the method so that the empty JsonObject is being filled,
	// if the object is valid it will return a 1. Otherwise 2.
	calvinMini.handleSetupTunnel(msg ,request, policy);

	// Checks if the JsonObject policy is size 0
	EXPECT_EQ(0, request.get("policy").size());
}

#endif
