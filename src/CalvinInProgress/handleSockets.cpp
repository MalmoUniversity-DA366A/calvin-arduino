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


/**
 * Manual IP-configuration
 * Setting up Ethernet connection and multiple servers listening to different ports.
 * @param MAC-address of the Ethernet-shield and desired IP-address
 */

void HandleSockets::setupConnection(byte *macAdr, IPAddress ipAdr)
{
	int status = 0;
	Ethernet.begin(macAdr, ipAdr);
	testServer.begin();
}

/**
 * DHCP-request
 * Setting up Ethernet connection and multiple servers listening to different ports.
 * @param MAC-address of the Ethernet-shield.
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
 * Sends a message to a specific socket
 * @Param socket, message to send, length of the message
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
  for(int i = 0;i < sizeOfMsg;i++)
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

void HandleSockets::testLoop()
{
	uint8_t socketStat[MAX_SOCK_NUM];
	uint8_t connectStatus[MAX_SOCK_NUM];

	setupConnection(testMac, testIp);
	Serial.println(Ethernet.localIP());					//print local IP
	delay(500);											//wait 0.5 seconds

	while(1)
	{
		byte listening = 0;
		//loop through all sockets
		for (int i = 0; i < MAX_SOCK_NUM; i++)
		{
			uint8_t s = W5100.readSnSR(i);                  //socket status
			socketStat[i] = s;
			//print socket number
			Serial.print(i);
			//check connection status of socket:
			switch(s)
			{
				case(SnSR::CLOSE_WAIT):
					close(i);            	//close the socket
					connectStatus[i] = 0;  //connection status for socket  = 0
					break;
				case(SnSR::LISTEN):
					listening = 1;      //waiting for connection?
					break;
				case(SnSR::ESTABLISHED):
						if(connectStatus[i] == 0)
						{
							connectStatus[i] = 1;                   //connectionStatus for socket = 1  --  behövs kankse inte?
							//lägg till socket i listan för inkommande anslutningar
						}
					break;
				default:
					break;
			}

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

			//check if there is anyone trying to send
			if(W5100.getRXReceivedSize(i) > 0)
			{
				if ( s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::CLOSE_WAIT )
				{
					//nothing to read
				}
				else if(socketIPAdr[0] != 0  && SnSR::ESTABLISHED)	//If IPAdresss doesn't start with 0
				{
					String recievedMsg = recvMsg(i);
					Serial.println(recievedMsg);
					String msg = "hej på dig}";
					Serial.println(msg.c_str());
					sendMsg(i, msg.c_str(), msg.length());
				}
			}

		} //end i < sock max

		Serial.println();

		//VAD GÖR DETTA???
		if(!listening)
		{
		    Serial.println("Not listening");
		    for(int i = 0;i < MAX_SOCK_NUM; i++)
		    {
		    	if(socketStat[i] == 0)
		    	{
		    		socket(i,SnMR::TCP,testPort,0);
		    		listen(i);
		    		break;
		    	}
		    }
		}
		delay(3000);					//3 second delay
	}	//end While(1)

}
