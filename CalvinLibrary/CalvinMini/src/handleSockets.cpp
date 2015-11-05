
/**
 * HandleSockets.cpp
 *
 *  Created on: 13 okt. 2015
 *      Author: Andreas
 */
#include <SPI.h>
#include <Ethernet.h>
#include <utility/w5100.h>
#include <utility/socket.h>
#include "HandleSockets.h"
// ------------- This should be set from the sketch: ---------------------
uint16_t testPort = 5002;
EthernetServer testServer(testPort);
//------------------------------------------------------------------------

const uint8_t messagesOutLenght = MAX_NBR_OF_SOCKETS*NBR_OF_OUTGOING_MSG;		//to keep track of the maximum amount of outgoing messages
BYTE listening = 0;

// Various list for handling sockets, incoming and outgoing messages:
uint8_t socketStat[MAX_NBR_OF_SOCKETS];
uint8_t connectStatus[MAX_NBR_OF_SOCKETS] = {0, 0, 0, 0};
uint8_t socketConnectionList[MAX_NBR_OF_SOCKETS]= {SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED};
String messagesIn[MAX_NBR_OF_SOCKETS];
String messagesOut[messagesOutLenght];

// Stores message counters for all sockets:
uint8_t messageCounters[MAX_NBR_OF_SOCKETS] = {0, 0, 0, 0};

void HandleSockets::setupConnection(BYTE *macAdr, IPAddress ipAdr)
{
	Ethernet.begin(macAdr, ipAdr);
	testServer.begin();
}

uint8_t HandleSockets::setupConnection(BYTE *macAdr)
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

uint8_t HandleSockets::getSocketConnectionStatus(int socketNbr)
{
	uint8_t result = 1;
	if(socketConnectionList[socketNbr] == SOCKET_NOT_CONNECTED)
	{
		result = 0;
	}
	return result;
}

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

uint8_t HandleSockets::sendMsg(uint8_t socket, const char *str, uint16_t length)
{
	BYTE hex[4] = {};
	hex[0] = (length & 0xFF000000);
	hex[1] = (length & 0x00FF0000);
	hex[2] = (length & 0x0000FF00) / 0x000000FF;
	hex[3] = (length & 0x000000FF);
	send(socket,(unsigned char*)hex, 4);						// send length of message
	send(socket,(unsigned char*)str, length);					// send actual message
	if(length == (hex[2]*256 + hex[3]))
	    return 1;
	  else
	    return 0;
}

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
