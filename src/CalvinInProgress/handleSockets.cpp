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
 *
 */

int HandleSockets::setupConnection(byte *macAdr, IPAddress ipAdr)
{
	int status = 0;
	Ethernet.begin(macAdr, ipAdr);
	testServer.begin();
	return status;
}

/**
 * DHCP-request
 * Setting up Ethernet connection and multiple servers listening to different ports.
 *
 */

int HandleSockets::setupConnection(byte *macAdr)
{
	int status = 0;
	if (Ethernet.begin(macAdr) == 0)
	    {
	        Serial.println("Failed to configure Ethernet using DHCP");
	        // Set try again if fail:
	        if (Ethernet.begin(macAdr) == 0)
	        {
	        	return status;
	        }
	    }
	status = 1;
	return status;
}


String HandleSockets::recvMsg(uint8_t socket)
{
  Serial.println("\nReading...");
  uint8_t tempBuff[MAX_LENGTH+1] = {};
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  int sizeOfMsg;
  int found = 0;

  sizeOfMsg = recvAvailable(socket);			//recieve lenght of incomming message
  /*
  if(sizeOfMsg > 4)
  {
	  sizeOfMsg = sizeOfMsg-4;						//remove the length header of the message
  }
  */
  Serial.print("SIZE of message:   ");
  Serial.println(sizeOfMsg);
  //read all incomming data one by one
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
	uint8_t listening = 0;

	setupConnection(testMac, testIp);
	Serial.println(Ethernet.localIP());					//print local IP
	delay(500);											//wait 0.5 seconds

	while(1)
	{
		//loop through all sockets
		for (int i = 0; i < MAX_SOCK_NUM; i++)
		{
			uint8_t s = W5100.readSnSR(i);                  //socket status
			socketStat[i] = s;
			//print socket number
			Serial.print(i);


			//if client is disconnected?
			if(s == SnSR::CLOSE_WAIT) 				//client dissconected waiting for close?
			{
				close(i);            	//close the socket
				connectStatus[i] = 0;  //connection status for socket  = 0
			}

			// if status = waiting for connection
			if(s == SnSR::LISTEN)
			{
				listening = 1;      //waiting for connection?
			}

			if((s == SnSR::ESTABLISHED) && (connectStatus[i] == 0)) 		//client connected?
			{
				connectStatus[i] = 1;                   //connectionStatus for socket = 1  --  behövs kankse inte?
				//lägg till socket i listan för inkommande anslutningar
			}

			//behver nog inte sparas någonstanns sålänge vi inte kör med flera olika inkommande portar (aka flera servrar)
			W5100.readSnPORT(i);						// connected sockets incoming port port


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
			uint8_t socktIPAdr[4];
			W5100.readSnDIPR(i, socktIPAdr);
			for (int j=0; j<4; j++)
			{
			  Serial.print(socktIPAdr[j],10);
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

			//If IPAdresss doesnt start with 0
			//if((socktIPAdr[0] != 0 )&& (s == SnSR::ESTABLISHED))
			if ( s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::CLOSE_WAIT )
			{
				//nothing to read
			}
			else if(socktIPAdr[0] != 0 )
			{
				String recievedMsg = recvMsg(i);
				Serial.println(recievedMsg);
				send(i,(unsigned char*)"ok ",3);
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
		delay(3000);
	}	//end While(1)

}
