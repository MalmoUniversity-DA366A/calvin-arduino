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

using namespace std;
class HandleSockets
{
public:
	int setupConnection(byte *macAdr, IPAddress ipAdr);
	int setupConnection(byte *macAdr);
	int sendMsg(uint8_t socket, const char *str);
	String recvMsg(uint8_t socket);
	void testLoop();

};

#endif /* CALVININPROGRESS_HANDLESOCKETS_H_ */
