/**
 * This file contains functions related to communication to and from
 * Calvin Arduino.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015
 *
 * Andreas Elvstam, Jesper Hansen, Peter Johansson, Daniel Nordahl, Carl �str�m
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CALVININPROGRESS_HANDLESOCKETS_H_
#define CALVININPROGRESS_HANDLESOCKETS_H_

#include <inttypes.h>
#include "ArduinoJson.h"
typedef unsigned char BYTE;
#define MAX_LENGTH 				1
#define MAX_NBR_OF_SOCKETS 		MAX_SOCK_NUM
#define NBR_OF_OUTGOING_MSG 	4
#define SOCKET_NOT_CONNECTED 	255

using namespace std;
class HandleSockets
{
public:
	const char* EMPTY_STR = "_$EMPTY$_";
#ifdef ARDUINO
	/**
	 * Manual IP-configuration
	 * Setting up Ethernet connection and Ethernet server listening on specified port.
	 * @param byte* MAC-address of the Ethernet-shield,
	 * @param IPAddress desired IP-address
	 */
	void setupConnection(BYTE *macAdr, IPAddress ipAdr);
#endif
	/**
	 * DHCP-request
	 * Setting up Ethernet connection and Ethernet server listening on specified port.
	 * If first attempt fails, try another time before quitting.
	 * @param byte* MAC-address of the Ethernet-shield.
	 * @return returns 1 if success, 0 if failed to  IP-address through DHCP
	 */
	uint8_t setupConnection(BYTE *macAdr);

	/**
	 * Check if anyone is connected.
	 * 0 if no socket i connected, 1 if connected
	 * @return uint8_t status
	 */
	uint8_t anyoneConnected(void);

	/**
	 * Sends a message to a specific socket.
	 * First sends the length in a separate packet then sends the actual message afterwards.
	 * @param uint8_t socket
	 * @param const char* message to send
	 * @param uint16_t length of the message
	 * @return uint8_t Check if length is handled right
	 */
	uint8_t sendMsg(uint8_t socket, const char *str, uint16_t length);

	/**
	 * Sends all outgoing messages stored in messagesOut[] to corresponding socket.
	 * Also resets the message counter to 0 for the corresponding socket.
	 * @param uint8_t socket
	 */
	void sendAllMsg(uint8_t socket);

	/**
	 * Receives a message from a specific socket.
	 * Begins message with a '{' to comply with the JSON TCP communication of Calvin Base
	 * @param uint8_t socket
	 * @return returns the string received from socket
	 */
	String recvMsg(uint8_t socket);

	/**
	 * Returns the message stored in messagesIn on msgIndex
	 * @param uint8_t msgIndex
	 * @return String message
	 */
	String getMessagesIn(uint8_t index);

	/**
	 * Adds message to outgoing message list.
	 * Returns 255 if messageOut[] is full, else the next index to place the outgoing message in.
	 * @param String reply, uint8_t socket
	 * @return uint8_t the next place to store outgoing message for specified socket
	 */
	uint8_t addToMessagesOut(String reply, uint8_t socket);

	/**
	 * Makes sure messagesOut and messagesIn is properly filled with the EMPTY_STR string.
	 */
	void prepareMessagesLists(void);

	/**
	 * Loops through all sockets to see if anything is to be read.
	 * If data was received the message is stored in to messagesIn array.
	 * If no data found specific string to determine this is added to the corresponding index.
	 * @return uint8_t status, 0 if nothing received, 1 if anything was received.
	 */
	uint8_t recvAllMsg(void);

	/**
	 * Loops through all sockets and determine their connection status.
	 * W5100 and the Ethernet library has built in support for 4 sockets,
	 * hence the maximum connected sockets cannot exceed 4 without modification to the Ethernet library.
	 * If connected, stores the corresponding socket in socketConnectionList[].
	 * Else stores 255 if socket is disconnected
	 */
	void determineSocketStatus(void);

	/**
	 * Controls which port to listen on next in order to add another socket once it becomes available.
	 * Simply chooses the next available socket with the lowest number.
	 */
	void NextSocket(void);

	/**
	 * Determines if a specific socket is connected or not
	 * @Param uint8_t socketNbr, the socket to control.
	 * @Return int status, 1 if connected, else 0.
	 */
	uint8_t getSocketConnectionStatus(int);

};

#endif /* CALVININPROGRESS_HANDLESOCKETS_H_ */
