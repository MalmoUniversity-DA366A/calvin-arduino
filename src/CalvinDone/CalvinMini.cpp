/**
 * This is the Calvin mini library for Arduino due
 *
 *Created on: 5 okt. 2015
 *
 */
#include <stdio.h>
#include "CalvinMini.h"
#include <inttypes.h>

#ifdef ARDUINO
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include "HandleSockets.h"
#include "Actors.h"
#include "Fifo.h"

byte mac[] = { 0x00, 0xAA, 0xAB, 0xCB, 0x0E, 0x02 };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress ip(192,168,0,99);
//IPAddress ip(192,168,0,10);
uint16_t slaveport = 5002;
EthernetServer server(slaveport);
EthernetClient client;
LiquidCrystal lcdOutMain(52, 50, 48, 46, 44, 42);
HandleSockets socketHandler;
#endif
uint32_t lastPop[4];
actor actors[NUMBER_OF_SUPPORTED_ACTORS];
uint8_t activeActors;
uint8_t nextMessage;
uint32_t sequenceNbr;


CalvinMini::CalvinMini()
{
	nextMessage = 0;
	sequenceNbr = 0;
	activeActors = 0;
}

rStatus CalvinMini::createActor(JsonObject &msg){
	rStatus allOk = FAIL;
	actor newActor;
	JsonObject &state = msg.get("state");
	JsonObject &name = state.get("actor_state");
	newActor.type = state.get("actor_type").asString();
	newActor.name = name.get("name");
	newActor.id = name.get("id");
	JsonObject &fifoObject = name["outports"]["integer"]["fifo"];
	JsonArray &fifoArray = fifoObject.get("fifo");
	JsonObject &count = fifoArray.get(0);
	newActor.count = (uint32_t)count.get("data");
	if( (activeActors < NUMBER_OF_SUPPORTED_ACTORS))
	{
		actors[activeActors] = newActor;
	}
	else
	{
		return FAIL;
	}
	actorInit(&actors[activeActors]);
	++activeActors;
	allOk = SUCCESS;
	return allOk;
}

actorType CalvinMini::getActorType(actor *inputActor)
{
	actorType ret;
	if(!strcmp(inputActor->type.c_str(),"io.StandardOut"))
	{
		ret = STD_ACTOR;
	}
	else if(!strcmp(inputActor->type.c_str(),"std.Counter"))
	{
		ret = COUNT_ACTOR;
	}
	else
	{
		ret = UNKNOWN_ACTOR;
	}
	return ret;
}

int8_t CalvinMini::getActorPos(const char* actorName,actor *list)
{
	for(int i = 0;i < NUMBER_OF_SUPPORTED_ACTORS;i++)
	{
		if(!strcmp(list[i].type.c_str(),actorName))
		{
			return i;
		}
	}
	return -1;					//Not found
}

void CalvinMini::initActorList()
{
	for(int i = 0;i < NUMBER_OF_SUPPORTED_ACTORS;i++)
	{
		actor emptyActor;
		emptyActor.type = "empty";
		actors[i] = emptyActor;
	}
}

rStatus CalvinMini::process(uint32_t token)
{
	rStatus allOk;
	int8_t pos;
	allOk = FAIL;
	for(int i = 0;i < NUMBER_OF_SUPPORTED_ACTORS;i++)
	{
	    if(!strcmp(actors[i].type.c_str(),"io.StandardOut"))
	    {
	        pos = i;
	    }
	    else if(!strcmp(actors[i].type.c_str(),"io.MovementStandardOut"))
	    {
	        pos = i;
	    }
	    else if(!strcmp(actors[i].type.c_str(),"io.LEDStandardOut"))
	    {
	    	Serial.print("process");
	    	Serial.println(i);
	        pos = i;
	    }
	}
	//pos = getActorPos("io.StandardOut",actors);

	allOk = fifoAdd(&actors[pos].inportsFifo[0],token);
	return allOk;
}

void CalvinMini::handleToken(JsonObject &msg, JsonObject &reply)
{
	rStatus fifoStatus;
	JsonObject &token = msg.get("token");
	fifoStatus = process((uint32_t)token.get("data"));
	reply.set("cmd",      "TOKEN_REPLY");
	reply.set("sequencenbr",  msg.get("sequencenbr"));
	reply.set("port_id",    msg.get("port_id"));
	reply.set("peer_port_id",   msg.get("peer_port_id"));
	if(fifoStatus == SUCCESS)
	{
		reply.set("value",      "ACK");
	}
	else
	{
		reply.set("value",      "NACK");  //Fifo is full, please come again
	}
	Serial.println("handleToken is done");
}

void CalvinMini::sendToken(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket, uint8_t nextSequenceNbr)
{
	int8_t pos;
	String str;
	for(int i= 0; i < NUMBER_OF_SUPPORTED_ACTORS; i++)
	{
	    if(!strcmp(actors[i].type.c_str(),"std.Counter") || !strcmp(actors[i].type.c_str(),"std.MovementSensor") || !strcmp(actors[i].type.c_str(),"std.RFID"))
	    {
	        pos = i;
	    }
	}
	actors[pos].ackFlag = nextSequenceNbr;								// Determines if ACK or NACK
#ifdef _MOCK_
	pos = 0;
#endif

	if(nextSequenceNbr)					//if ACK
	{
		sequenceNbr++;
		lastPop[socket] = fifoPop(&actors[pos].inportsFifo[0]);
		request.set("data", lastPop[socket]);
	}
	else if(!nextSequenceNbr)			// if NACK
	{
		request.set("data", lastPop[socket]);
	}
	request.set("type", "Token");
	reply.set("sequencenbr", sequenceNbr);
	reply.set("token", request);
	reply.set("cmd", "TOKEN");
	reply.set("port_id", actors[pos].port_id);
	reply.set("peer_port_id", actors[pos].peer_port_id);
}

void CalvinMini::handleTunnelData(JsonObject &msg, JsonObject &reply,JsonObject &request, uint8_t socket)
{
	JsonObject &value = msg.get("value");
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject &token = jsonBuffer.createObject();
	reply.set("to_rt_uuid",   msg.get("from_rt_uuid"));
	reply.set("from_rt_uuid",   RT_ID);
	reply.set("cmd",      "TUNNEL_DATA");
	reply.set("tunnel_id",    tunnel_id); // None in python
	if(!strcmp(value.get("cmd"), "TOKEN_REPLY") && !strcmp(value.get("value"), "NACK"))
	{
		sendToken(value, request, token, socket, 0);
	}
	else if(!strcmp(value.get("cmd"), "TOKEN"))
	{
#ifdef ARDUINO
		handleMsg(value,reply,request, socket);
#endif
	}
	else
	{
		sendToken(value, request, token, socket, 1);
	}
	reply.set("value", request);
}

void CalvinMini::handleActorNew(JsonObject &msg, JsonObject &reply, uint8_t socket)
{
	createActor(msg);
	reply.set("cmd",      "REPLY");
	reply.set("msg_uuid",   msg.get("msg_uuid"));
	reply.set("value",      "ACK");
	reply.set("from_rt_uuid", RT_ID);
	reply.set("to_rt_uuid",   msg.get("from_rt_uuid"));
}

void CalvinMini::handleSetupPorts(JsonObject &msg,JsonObject &request, uint8_t socket)
{
	JsonObject &inports = msg["state"]["prev_connections"]["inports"];
	JsonObject &outports = msg["state"]["prev_connections"]["outports"];

	int8_t pos;
	for(int i= 0; i < NUMBER_OF_SUPPORTED_ACTORS; i++)
	{
	      if(!strcmp(actors[i].type.c_str(),"std.Counter") || !strcmp(actors[i].type.c_str(),"std.MovementSensor") || !strcmp(actors[i].type.c_str(),"std.RFID"))
	      {
	          pos = i;
	      }
	}
#ifdef _MOCK_
	pos = 0;
#endif
	String port_id;
	String peer_port_id;
	if(outports.size() == 0)
	{
		JsonObject &token = msg["state"]["actor_state"]["inports"]["token"];
		port_id = token.get("id").asString();
		JsonArray &array = inports[port_id];
		peer_port_id = array.get(1).asString();
	}
	if(inports.size() == 0)
	{
		JsonObject &integer = msg["state"]["actor_state"]["outports"]["integer"];
		port_id = integer.get("id").asString();
		JsonArray &outerarray = outports[port_id];
		JsonArray &innerarray = outerarray.get(0);  // Inception
		peer_port_id = innerarray.get(1).asString();
	}
	request.set("msg_uuid","MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb7");
	request.set("from_rt_uuid", RT_ID);
	request.set("to_rt_uuid",msg.get("from_rt_uuid"));
	request.set("port_id", port_id);
	request.set("peer_port_id", peer_port_id);
	request.set("peer_actor_id", 0);
	request.set("peer_port_name", 0);
	request.set("peer_port_dir", 0);
	request.set("tunnel_id", tunnel_id);
	request.set("cmd", "PORT_CONNECT");
	actors[pos].peer_port_id = peer_port_id;
	actors[pos].port_id = port_id;
}

int8_t CalvinMini::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket)
{
	int8_t pos;
	if(!strcmp(msg.get("cmd"),"JOIN_REQUEST"))
	{
		  // JsonObject for replying a join request
		  StaticJsonBuffer<200> jsonBuffer;
		  JsonObject &policy = jsonBuffer.createObject();
		  handleJoin(msg,reply, socket);
		  handleSetupTunnel(msg, request, policy);
		  uint8_t moreThanOneMsg = 1;
		  // Print JsonObject and send to Calvin
		  uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
#ifdef ARDUINO
		  lcdOutMain.clear();
		  lcdOutMain.write("JOIN_REQUEST");
#endif
		  return size;
	}
	else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
	{
		handleActorNew(msg, reply, socket);
		handleSetupPorts(msg, request, socket);

		uint8_t moreThanOneMsg = 1;
		uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
		return size;
	}
	else if(!strcmp(msg.get("cmd"),"TUNNEL_DATA"))
	{
		handleTunnelData(msg, reply, request, socket);

		uint8_t moreThanOneMsg = 0;
		uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
		return size;
	}
	else if(!strcmp(msg.get("cmd"),"TOKEN"))
	{
		Serial.println("handleMsg: TOKEN");
		handleToken(msg,request);
		return 4;
	}
	else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
	{
		return 5;
	}
	else if(!strcmp(msg.get("cmd"),"REPLY"))
	{
	    for(int i= 0; i < NUMBER_OF_SUPPORTED_ACTORS; i++)
	    {
	        if(!strcmp(actors[i].type.c_str(),"std.Counter") || !strcmp(actors[i].type.c_str(),"std.MovementSensor") || !strcmp(actors[i].type.c_str(),"std.RFID"))
	        {
	            handleTunnelData(msg, reply, request, socket);
	            uint8_t moreThanOneMsg = 0;
	            uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
	            return size;
	        }
	    }
	    return 6;
	}
	else
	{
#ifdef ARDUINO
		Serial.println("UNKNOWN CMD");
		standardOut("UNKNOWN CMD");
#endif
		return 7;
	}
}

uint8_t CalvinMini::packMsg(JsonObject &reply, JsonObject &request, uint8_t moreThanOneMsg, uint8_t socket)
{
#ifdef _MOCK_
  nextMessage = 0;
#endif
	char replyTemp[2048] = {};
	reply.printTo(replyTemp,2048);
	String str(replyTemp);
	uint8_t size = addToMessageOut(str, socket);
	if(moreThanOneMsg)
	{
		char requestTemp[2048] = {};
		request.printTo(requestTemp,2048);
		String str2(requestTemp);
		size = addToMessageOut(str2, socket);
	}
	return size;
}

uint8_t CalvinMini::addToMessageOut(String reply, uint8_t socket)
{
#ifdef ARDUINO
	nextMessage = socketHandler.addToMessagesOut(reply, socket);
#endif
#ifdef _MOCK_
	nextMessage++;
#endif
	return nextMessage;
}

void CalvinMini::handleJoin(JsonObject &msg, JsonObject &reply, uint8_t socket)
{
	reply["cmd"] = "JOIN_REPLY";
	reply["id"] = RT_ID;
	reply["sid"] = msg.get("sid");
	reply["serializer"] = "json";
}

void CalvinMini::handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy)
{
	request["msg_uuid"] = "MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb6"; // Should be a unique id
	request["from_rt_uuid"] = RT_ID;
	request["to_rt_uuid"] = msg.get("id");
	request["cmd"] = "TUNNEL_NEW";
	request["tunnel_id"] = tunnel_id;
	request["policy"] = policy; // Unused
	request["type"] = "token";
}

#ifdef ARDUINO

void CalvinMini::calibrateSensor(void)
{
  lcdOutMain.write("Calibrating");
  for(int i = 0; i < calibrationTime; i++)
  {
      lcdOutMain.write(".");
      delay(1000);
  }
  lcdOutMain.clear();
  delay(50);
}

void CalvinMini::loop()
{
  calibrateSensor();
	lcdOutMain.write("Hello Calvin");
	initActorList();
	//------------This should be set from within the skecth later on:-----------------
	socketHandler.setupConnection(mac, ip);
	//--------------------------------------------------------------------------------
	socketHandler.prepareMessagesLists();
	delay(500);
	while(1)
	{
		socketHandler.determineSocketStatus();										// 1: Check connected sockets
		if(socketHandler.anyoneConnected() == 1)									// Anyone connected?
		{
			if(socketHandler.recvAllMsg() == 1)										// 3: Read message
			{
				for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)							// 4: Handle message
				{
					String str = socketHandler.getMessagesIn(i);
					const char* message = str.c_str();
					if(strncmp(socketHandler.EMPTY_STR, message, 9)!= 0)
					{
						StaticJsonBuffer<4096> jsonBuffer;
						JsonObject &msg = jsonBuffer.parseObject(str.c_str());
						JsonObject &reply = jsonBuffer.createObject();
						JsonObject &request = jsonBuffer.createObject();
						handleMsg(msg, reply, request, i);

						for(int i = 0;i < NUMBER_OF_SUPPORTED_ACTORS;i++)			// 5: Fire actors
						{
							if(strcmp(actors[i].type.c_str(),"empty") && actors[i].ackFlag)
							{
							    actors[i].fire(&actors[i]);
							}
						}
					}
				}
			}

			for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)								// 6: Send outgoing message
			{
				socketHandler.sendAllMsg(i);
			}
		}
		socketHandler.NextSocket();
	}
}
#endif
