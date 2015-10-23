/*
 * handleSockets.cpp
 *
 *  Created on: 13 okt. 2015
 *      Author: Andreas
 */

#include <SPI.h>
#include <Ethernet.h>
#include "handleSockets.h"
#include <utility/w5100.h>
#include <utility/socket.h>

// ------------- This should be set from the sketch: ---------------------
byte testMac[] = { 0xAA, 0xBB, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress testIp(192,168,0,10);

uint16_t testPort = 5002;
EthernetServer testServer(testPort);
//------------------------------------------------------------------------

const uint8_t messagesOutLenght = MAX_NBR_OF_SOCKETS*NBR_OF_OUTGOING_MSG;		//to keep track of the maximum amount of outgoing messages
byte listening = 0;

// Various list for handling sockets, incoming and outgoing messages:
uint8_t socketStat[MAX_NBR_OF_SOCKETS];
uint8_t connectStatus[MAX_NBR_OF_SOCKETS] = {0, 0, 0, 0};
uint8_t socketConnectionList[MAX_NBR_OF_SOCKETS]= {SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED};
String messagesIn[MAX_NBR_OF_SOCKETS];
String messagesOut[messagesOutLenght];

// Stores message counters for all sockets:
uint8_t messageCounters[MAX_NBR_OF_SOCKETS] = {0, 0, 0, 0};


/**
 * Manual IP-configuration
 * Setting up Ethernet connection and Ethernet server listening on specified port.
 * @param byte* MAC-address of the Ethernet-shield, IPAddress desired IP-address
 */
void HandleSockets::setupConnection(byte *macAdr, IPAddress ipAdr)
{
	Ethernet.begin(macAdr, ipAdr);
	testServer.begin();
}

/**
 * DHCP-request
 * Setting up Ethernet connection and Ethernet server listening on specified port.
 * If first attempt fails, try another time before quitting.
 * @param byte* MAC-address of the Ethernet-shield.
 * @return returns 1 if success, 0 if failed to  IP-address through DHCP
 */
int HandleSockets::setupConnection(byte *macAdr)
{
	int status = 0;
	if (Ethernet.begin(macAdr) == 0)
	    {
	        if (Ethernet.begin(macAdr) == 0)		// try again if fail
	        {
	        	Serial.println("Failed to configure Ethernet using DHCP");
	        	return status;
	        }
	    }
	status = 1;
	return status;
}

/**
 * Makes sure messagesOut and messagesIn is properly filled with the EMPTY_STR string.
 */
void HandleSockets::prepareMessagesLists()
{
	for(int j = 0; j < messagesOutLenght; j++)
	{
		messagesOut[j] = EMPTY_STR;
	}
	for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)
	{
		messagesIn[i] = EMPTY_STR;
	}
}

/**
 * Sends a message to a specific socket.
 * First sends the length in a separate packet then sends the actual message afterwards.
 * @Param uint8_t socket, const char* message to send, uint16_t length of the message
 */
void HandleSockets::sendMsg(uint8_t socket, const char *str, uint16_t length)
{
	BYTE hex[4] = {};
	hex[0] = (length & 0xFF000000);
	hex[1] = (length & 0x00FF0000);
	hex[2] = (length & 0x0000FF00) / 0x000000FF;
	hex[3] = (length & 0x000000FF);
	send(socket,(unsigned char*)hex, 4);						// send length of message
	send(socket,(unsigned char*)str, length);					// send actual message
}

/**
 * Sends all outgoing messages stored in messagesOut[] to corresponding socket.
 * Also resets the message counter to 0 for the corresponding socket.
 * @param uint8_t socket
 */
void HandleSockets::sendAllMsg(uint8_t socket)
{
	uint8_t startingPoint = socket * NBR_OF_OUTGOING_MSG;
	for(int j = 0; j < NBR_OF_OUTGOING_MSG; j++)					// loop all messages
	{
		const char* message = messagesOut[startingPoint+j].c_str();
		if(strncmp(EMPTY_STR, message, 9)!= 0)						// anything to send?
		{
			sendMsg(socket, messagesOut[startingPoint+j].c_str(), messagesOut[startingPoint+j].length());
			messagesOut[startingPoint+j] = EMPTY_STR;
		}
		flush(socket);												// flushes outgoing messages to reduce latency
	}
	messageCounters[socket] = 0;
}

/**
 * Receives a message from a specific socket.
 * Begins message with a '{' to comply with the JSON TCP communication of Calvin Base
 * @Param uint8_t socket
 * @return returns the string received from socket
 */
String HandleSockets::recvMsg(uint8_t socket)
{
  uint8_t tempBuff[MAX_LENGTH+1] = {};
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  int sizeOfMsg= recvAvailable(socket);			//Receive length of incoming message
  int found = 0;

  for(int i = 0; i < sizeOfMsg; i++)			//read all incoming data one by one
  {
      int size = recv(socket,tempBuff, MAX_LENGTH);
      if(size == 0 || size == -1)
      {
    	  str = EMPTY_STR;
    	  return str;
      }
      else
      {
    	  uint8_t tempChar = tempBuff[0];
    	  temp[0] = tempChar;
    	  if(temp[0] == '{')
    	  {
    		 found = 1;
    	  }
    	  if(found == 1)
    	  {
			  temp[size] = '\0';  // Null terminate char
			  str += temp;
    	  }
      }
  }
  if(found == 1)
  {
	  return str;
  }
  else {
	  return EMPTY_STR;
  }
}

/**
 * Returns the message stored in messagesIn on msgIndex
 * @param uint8_t msgIndex
 * @return String message
 */
String HandleSockets::getMessagesIn(uint8_t msgIndex)
{
	String reply = EMPTY_STR;
	if(msgIndex < MAX_NBR_OF_SOCKETS)
	{
		reply = messagesIn[msgIndex];
		messagesIn[msgIndex] = EMPTY_STR;
	}
	return reply;
}

/**
 * Check if anyone is connected.
 * 0 if no socket i connected, 1 if connected
 * @return uint8_t status
 */
uint8_t HandleSockets::anyoneConnected()
{
	uint8_t status = 0;
	if(socketConnectionList[0] != SOCKET_NOT_CONNECTED)
	{
			status = 1;
	}
	if(socketConnectionList[1] != SOCKET_NOT_CONNECTED)
	{
			status = 1;
	}
	if(socketConnectionList[2] != SOCKET_NOT_CONNECTED)
	{
			status = 1;
	}
	if(socketConnectionList[3] != SOCKET_NOT_CONNECTED)
	{
			status = 1;
	}
	return status;
}

/**
 * Adds message to outgoing message list.
 * Returns 255 if messageOut[] is full, else the next index to place the outgoing message in.
 * @param String reply, uint8_t socket
 * @return uint8_t the next place to store outgoing message for specified socket
 */
uint8_t HandleSockets::addToMessagesOut(String reply, uint8_t socket)
{
	uint8_t replyNbr = 255;							//255 equals error in this case
	if(messageCounters[socket]<NBR_OF_OUTGOING_MSG)
	{
		messagesOut[messageCounters[socket]] = reply;
		messageCounters[socket] = messageCounters[socket]+1;
		if(messageCounters[socket]<NBR_OF_OUTGOING_MSG)
		{
			replyNbr = messageCounters[socket];
		}
	}
	return replyNbr;
}

/**
 * Loops through all sockets to see if anything is to be read.
 * If data was received the message is stored in to messagesIn array.
 * If no data found specific string to determine this is added to the corresponding index.
 * @return uint8_t status, 0 if nothing received, 1 if anything was received.
 */
uint8_t HandleSockets::recvAllMsg()
{
	uint8_t result = 0;
	for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)					// Loop sockets
	{
		if(socketConnectionList[i] != SOCKET_NOT_CONNECTED)		// Connected?
		{
			messagesIn[i] = recvMsg(socketConnectionList[i]);
			result = 1;
		}
		else {													// Not connected
			messagesIn[i] = EMPTY_STR;
		}
	}
	return result;
}

/**
 * Loops through all sockets and determine their connection status.
 * W5100 and the Ethernet library has built in support for 4 sockets,
 * hence the maximum connected sockets cannot exceed 4 without modification to the Ethernet library.
 * If connected, stores the corresponding socket in socketConnectionList[].
 * Else stores 255 if socket is disconnected
 */
void HandleSockets::determineSocketStatus()
{
	listening = 0;
	for (int i = 0; i < MAX_NBR_OF_SOCKETS; i++)						// loop all sockets
	{
		uint8_t s = W5100.readSnSR(i);                  				// socket status
		socketStat[i] = s;
		switch(s)														// determine connection status of socket
		{
			case(SnSR::CLOSED):											// socket closed/available
				socketConnectionList[i] = SOCKET_NOT_CONNECTED;			// remove from list
				break;
			case(SnSR::CLOSE_WAIT):										// waiting for close?
				close(i);            									// close the socket
				connectStatus[i] = 0;
				socketConnectionList[i] = SOCKET_NOT_CONNECTED;
				break;
			case(SnSR::LISTEN):											// listening?
				listening = 1;
				break;
			case(SnSR::ESTABLISHED):									// connected?
				if(connectStatus[i] == 0)
				{
					connectStatus[i] = 1;
					socketConnectionList[i] = i;						// add to list
				}
				break;
			default:
				//none of the above fulfilled, do nothing here
				break;
		}
	} //end i < sock max
}

/**
 * Controls which port to listen on next in order to add another socket once it becomes available.
 * Simply chooses the next available socket with the lowest number.
 */
void HandleSockets::NextSocket()
{
	if(!listening)
	{
		for(int i = 0;i < MAX_NBR_OF_SOCKETS; i++)
		{
			if(socketStat[i] == 0)
			{
				socket(i,SnMR::TCP,testPort,0);
				listen(i);
				break;
			}
		}
	}
}

/**
 * Used for testing only the handleSockets functions:
 */
void HandleSockets::testLoop()
{
	setupConnection(testMac, testIp);
	Serial.println(Ethernet.localIP());					//print local IP
	delay(500);											//wait 0.5 seconds
	while(1)
	{
		determineSocketStatus();
		recvAllMsg();					//read incoming messages and save in messagesIn
		//sendAllMsg();					//måste ändras för att skcika med socket nummer om man skall använda denna
		Serial.println();
		delay(3000);					//3 second delay
	}	//end While(1)

}
