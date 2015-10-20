/*
 * handleSockets.h
 *
 *  Created on: 13 okt. 2015
 *      Author: Andreas
 */

#ifndef CALVININPROGRESS_HANDLESOCKETS_H_
#define CALVININPROGRESS_HANDLESOCKETS_H_

typedef unsigned char BYTE;
#define MAX_LENGTH 1
#define MAX_NBR_OF_SOCKETS MAX_SOCK_NUM
#define NBR_OF_OUTGOING_MSG 10
#define SOCKET_NOT_CONNECTED 255

using namespace std;
class HandleSockets
{
public:
	void setupConnection(byte *macAdr, IPAddress ipAdr);
	int setupConnection(byte *macAdr);
	void sendMsg(uint8_t socket, const char *str, uint16_t length);
	void sendAllMsg(void);
	String recvMsg(uint8_t socket);
	void recvAllMsg(void);
	void determineSocketStatus(void);
	void testLoop();

};

#endif /* CALVININPROGRESS_HANDLESOCKETS_H_ */
