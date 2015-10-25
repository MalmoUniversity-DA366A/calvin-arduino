
/*
 * testSendMsg.cpp
 *
 *      Author: Peter Johansson
 */
#ifdef _MOCK_

#include "gtest/gtest.h"
#include "../src/CalvinDone/CalvinMini.h"
#include "../src/CalvinDone/handleSockets.h"

class testSendMsg : public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST(testSendMsg, testShortLength)
{
    HandleSockets socket;
    String str = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[1024,\"inside\"]}";
    int size = socket.sendMsg(0, str.c_str(), str.length());

    // Check if size of 8-bit string
    // is same as outgoing message size
    EXPECT_EQ(1, size);
}
TEST(testSendMsg, testLongLength)
{
    HandleSockets socket;
    String str = "{\"msg_uuid\":\"MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb7\""
        ",\"from_rt_uuid\":\"calvin-arduino\",\"to_rt_uuid\":\"51231c6f-4ebf-49b4-a565-02679dd2fa6b\""
        ",\"port_id\":\"0c2538ef-4a37-4cf8-acc8-4f750e2bb0a8\",\"peer_port_id\":\"a655527a-926e-4c17-bd85-173fb91de36f\""
        ",\"peer_actor_id\":0,\"peer_port_name\":0,\"peer_port_dir\":0,\"tunnel_id\":\"fake-tunnel\",\"cmd\":\"PORT_CONNECT\"}";
    int size = socket.sendMsg(0, str.c_str(), str.length());

    // Test size of string over 8-bit
    // is same as outgoing message size
    EXPECT_EQ(1, size);
}
#endif
