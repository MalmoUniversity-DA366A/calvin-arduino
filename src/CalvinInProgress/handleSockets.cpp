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
 * Sends a message to a specific socket.
 * @Param uint8_t socket, const char* message to send, uint16_t length of the message
 * @return returns 1 if success, 0 if failed
 */
void HandleSockets::sendMsg(uint8_t socket, const char *str, uint16_t length)
{
	Serial.println("Sending...");
	Serial.print("size of sent msg: ");
	Serial.println(length);
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
		if(strncmp("_$EMPTY$_", messagesOut[j],9 ))						//anything to send?
		{
			socketNbr = j/NBR_OF_OUTGOING_MSG;
			sendMsg(socketNbr, messagesOut[j].c_str(), messagesOut[j].length());
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
  Serial.println("\nReading...");
  uint8_t tempBuff[MAX_LENGTH+1] = {};
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  int sizeOfMsg;
  int found = 0;

  sizeOfMsg = recvAvailable(socket);			//Receive length of incoming message
  //--------utskrifter----------
  Serial.print("SIZE of message:   ");
  Serial.println(sizeOfMsg);
  //-----------------------------
  //read all incoming data one by one
  for(int i = 0; i < sizeOfMsg; i++)
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
			Serial.print(":   ");
			Serial.println(messagesIn[i]);
			String msg = "hej på dig}";
			Serial.println(msg.c_str());
			sendMsg(i, msg.c_str(), msg.length());
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
	byte listening = 0;
	//loop through all sockets.
	for (int i = 0; i < MAX_NBR_OF_SOCKETS; i++)
	{
		uint8_t s = W5100.readSnSR(i);                  //socket status
		socketStat[i] = s;
		Serial.print(i);								//print socket number
		//determine connection status of socket:
		switch(s)
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
		//print socket status to terminal
		Serial.print(F(" :0x"));
		if(s < 16)
		{
			Serial.print(F("0"));
		}
		Serial.print(s,HEX);
		Serial.print(F(" "));
		Serial.print(W5100.readSnPORT(i));        //print the port that the client is connected to
		Serial.print(" ");

		//save the IpAddress in socktIPAdr -- Needed??
		uint8_t socketIPAdr[4];
		W5100.readSnDIPR(i, socketIPAdr);					//stores IPAddress in socktIPAdr
		for (int j=0; j<4; j++)
		{
		  Serial.print(socketIPAdr[j],10);
		  if (j<3)
		  {
			  Serial.print(".");
		  }
		}

		//skall sparas någonstans för att särskilja vem som är vem??
		//print the internal Port
		Serial.print("(");
		Serial.print(W5100.readSnDPORT(i));
		Serial.print(") ");
		Serial.println();
		//--------------------------------------------------------------------------------------------------------------

	} //end i < sock max

	//determine next socket to listen to
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
