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

byte testMac[] = { 0xAA, 0xBB, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress testIp(192,168,0,10);

uint16_t testPort = 5002;
EthernetServer testServer(testPort);


uint8_t socketStat[MAX_NBR_OF_SOCKETS];
uint8_t connectStatus[MAX_NBR_OF_SOCKETS] = {0, 0, 0, 0};

uint8_t socketConnectionList[MAX_NBR_OF_SOCKETS]= {SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED, SOCKET_NOT_CONNECTED};
String messagesIn[MAX_NBR_OF_SOCKETS];
const uint8_t messagesOutLenght = MAX_NBR_OF_SOCKETS*NBR_OF_OUTGOING_MSG;		//to keep track of the maximum amount of outgoing messages
String messagesOut[messagesOutLenght];
int socket0count = 0;
int socket1count = 0;
int socket2count = 0;
int socket3count = 0;
byte listening = 0;


/**
 * Manual IP-configuration
 * Setting up Ethernet connection and multiple servers listening to different ports.
 * @param byte* MAC-address of the Ethernet-shield, IPAddress desired IP-address
 */
void HandleSockets::setupConnection(byte *macAdr, IPAddress ipAdr)
{
	Ethernet.begin(macAdr, ipAdr);
	testServer.begin();
}

/**
 * DHCP-request
 * Setting up Ethernet connection and multiple servers listening to different ports.
 * @param byte* MAC-address of the Ethernet-shield.
 * @return returns 1 if success, 0 if failed to  IP-address through DHCP
 */
int HandleSockets::setupConnection(byte *macAdr)
{
	int status = 0;
	if (Ethernet.begin(macAdr) == 0)
	    {
	        Serial.println("Failed to configure Ethernet using DHCP");
	        // try again if fail:
	        if (Ethernet.begin(macAdr) == 0)
	        {
	        	return status;
	        }
	    }
	status = 1;
	return status;
}

/**
 * Makes sure messagesOut properly filled with empty strings.
 */
void HandleSockets::prepareMessagesOut()
{
	for(int j = 0; j < messagesOutLenght; j++)
		{
		messagesOut[j] = EMPTY_STR;
		}
}

/**
 * Sends a message to a specific socket.
 * @Param uint8_t socket, const char* message to send, uint16_t length of the message
 * @return returns 1 if success, 0 if failed
 */
void HandleSockets::sendMsg(uint8_t socket, const char *str, uint16_t length)
{
	Serial.println("Sending...");
	Serial.print("size of sent msg: ");
	Serial.println(length);

	BYTE hex[4] = {};
	hex[0] = (length & 0xFF000000);
	hex[1] = (length & 0x00FF0000);
	hex[2] = (length & 0x0000FF00) / 0x000000FF;
	hex[3] = (length & 0x000000FF);
	send(socket,(unsigned char*)hex, 4);

	send(socket,(unsigned char*)str, length);
}

/**
 * Sends all outgoing messages stored in messagesOut[] to corresponding socket.
 * the socket is determined by dividing the counter variable j by the number of outgoing messages each socket can have.
 * Since socketNbr is an uint8_t all decimal numbers from the division will be removed hence only 0 through 3 will be a valid outcome.
 * If message is determined as empty nothing is sent.
 */
void HandleSockets::sendAllMsg()
{
	uint8_t socketNbr = 0;
	for(int j = 0; j < messagesOutLenght; j++)
	{
		const char* message = messagesOut[j].c_str();
		if(strncmp(EMPTY_STR, message, 9)!= 0)						//anything to send?
		{
			socketNbr = j/NBR_OF_OUTGOING_MSG;
			sendMsg(socketNbr, messagesOut[j].c_str(), messagesOut[j].length());
			messagesOut[j] = EMPTY_STR;
		}
	}
}

/**
 * Receives a message from a specific socket
 * @Param socket
 * @return returns the string received from socket
 */
String HandleSockets::recvMsg(uint8_t socket)
{
  uint8_t tempBuff[MAX_LENGTH+1] = {};
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  int sizeOfMsg= recvAvailable(socket);			//Receive length of incoming message
  int found = 0;
  //--------utskrifter----------
  Serial.print("SIZE of message:   ");
  Serial.println(sizeOfMsg);
  //-----------------------------

  for(int i = 0; i < sizeOfMsg; i++)			//read all incoming data one by one
  {
      int size = recv(socket,tempBuff, MAX_LENGTH);
      if(size == 0 || size == -1)
      {
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
    	  if(found ==1)
    	  {
			  temp[size] = '\0';  // Null terminate char
			  str += temp;
    	  }
      }
  }
  return str;
}

/**
 * Returns the message stored in messagesIn on msgIndex
 * @param uint8_t msgIndex
 * @return String message
 */
String HandleSockets::getMessagesIn(uint8_t msgIndex)
{
	String reply;
	if(msgIndex < MAX_NBR_OF_SOCKETS)
	{
		reply = messagesIn[msgIndex];
	}
	return reply;
}

/**
 * Adds message to outgoing message list.
 * @param String reply, uint8_t socket
 * @return int the next place to store outgoing message
 */
int HandleSockets::addToMessagesOut(String reply, uint8_t socket)
{
	int rply = 0;
	switch(socket)
	{
	case(0):
			if(socket0count<10)
			{
				messagesOut[socket0count] = reply;
				socket0count = socket0count+1;
				rply = socket0count;
			}
			break;
	case(1):
			if(socket1count<10)
			{
				messagesOut[10+socket1count] = reply;
				socket1count = socket1count+1;
				rply = socket1count;
			}
			break;
	case(2):
			if(socket2count<10)
			{
				messagesOut[20+socket2count] = reply;
				socket2count = socket2count+1;
				rply = socket2count;
			}
			break;
	case(3):
			if(socket3count<10)
			{
				messagesOut[30+socket3count] = reply;
				socket3count = socket3count+1;
				rply = socket3count;
			}
			break;
	}
	return rply;
}

/**
 * Loops through all sockets to see if anything is to be read.
 * If data was received the message is stored in to messagesIn array.
 * If no data found specific string to determine this is added to the corresponding index.
 */
void HandleSockets::recvAllMsg()
{
	for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)
	{
		if(socketConnectionList[i] != SOCKET_NOT_CONNECTED)
		{
			messagesIn[i] = recvMsg(socketConnectionList[i]);
			Serial.print(i);
		}
		else {
			messagesIn[i] = "_$EMPTY$_";
		}
	}
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
	for (int i = 0; i < MAX_NBR_OF_SOCKETS; i++)		// loop all sockets
	{
		uint8_t s = W5100.readSnSR(i);                  // socket status
		socketStat[i] = s;
		Serial.print(i);								// print socket number
		switch(s)										// determine connection status of socket
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
		//--------------------------------------UTSKRIFTER-----------------------------------------------------
		//print the Destination Port, only for testing purposes
		Serial.print("(");
		Serial.print(W5100.readSnDPORT(i));
		Serial.print(") ");
		Serial.println();
		//--------------------------------------------------------------------------------------------------------------

	} //end i < sock max
}

/**
 * Controls which port to listen on next on order to add another socket once available.
 * Simply chooses the next available socket number.
 */
void HandleSockets::NextSocket()
{
	if(!listening)
	{
		Serial.println("Not listening");
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
		sendAllMsg();
		Serial.println();
		delay(3000);					//3 second delay
	}	//end While(1)

}
