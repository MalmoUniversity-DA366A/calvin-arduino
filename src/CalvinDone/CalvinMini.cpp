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

IPAddress ip;
byte mac[6];
uint16_t slaveport;
EthernetServer server(slaveport);
EthernetClient client;
LiquidCrystal lcdOutMain(52, 50, 48, 46, 44, 42);
HandleSockets socketHandler;
#endif
String RT_ID = "";
uint32_t lastPop[4];
actor actors[NUMBER_OF_SUPPORTED_ACTORS];
uint8_t activeActors;
uint8_t nextMessage[NUMBER_OF_SUPPORTED_ACTORS] = {0,0,0,0};
uint32_t sequenceNbr[NUMBER_OF_SUPPORTED_ACTORS] = {0,0,0,0};

#ifdef ARDUINO
CalvinMini::CalvinMini(String rtID, byte* macAdr, IPAddress ipAdr, uint16_t port)
{
	RT_ID = rtID;
	mac[0] = macAdr[0];
	mac[1] = macAdr[1];
	mac[2] = macAdr[2];
	mac[3] = macAdr[3];
	mac[4] = macAdr[4];
	mac[5] = macAdr[5];
	mac[6] = macAdr[6];
	ip = ipAdr;
	slaveport = port;
	activeActors = 0;
	Serial.println(ip);
}
#else
CalvinMini::CalvinMini(){
	RT_ID = "Calvin-arduino";
	activeActors = 0;
}
#endif

rStatus CalvinMini::createActor(JsonObject &msg, uint8_t socket){
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
		actors[socket] = newActor;
	}
	else
	{
		return FAIL;
	}
	actorInit(&actors[socket]);
	++activeActors;
	allOk = SUCCESS;
	return allOk;
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

rStatus CalvinMini::process(uint32_t token, uint8_t socket)
{
	rStatus allOk;
	int8_t pos;
	allOk = FAIL;
	if(!strncmp(actors[socket].type.c_str(),"io.",3))
	{
		pos = socket;
		allOk = fifoAdd(&actors[pos].inportsFifo[0],token);
	}
	return allOk;
}

void CalvinMini::handleToken(JsonObject &msg, JsonObject &reply, uint8_t socket)
{
	rStatus fifoStatus;
	JsonObject &token = msg.get("token");
	fifoStatus = process((uint32_t)token.get("data"), socket);
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
}

void CalvinMini::sendToken(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket, uint8_t nextSequenceNbr)
{
	int8_t pos;
	String str;
	if(!strncmp(actors[socket].type.c_str(),"std.",4))
	{
		pos = socket;
	}

#ifdef _MOCK_
	pos = 0;
#endif
	actors[pos].ackFlag = nextSequenceNbr;								// Determines if ACK or NACK

	if(nextSequenceNbr)					//if ACK
	{
		sequenceNbr[socket]++;
		lastPop[socket] = fifoPop(&actors[pos].inportsFifo[0]);
		request.set("data", lastPop[socket]);
	}
	else if(!nextSequenceNbr)			// if NACK
	{
		request.set("data", lastPop[socket]);
	}
	request.set("type", "Token");
	reply.set("sequencenbr", sequenceNbr[socket]);
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
	createActor(msg,socket);
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
	int8_t pos = socket;

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
	int8_t pos = socket;
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
		handleToken(msg,request,socket);
		return 4;
	}
	else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
	{
		return 5;
	}
	else if(!strcmp(msg.get("cmd"),"REPLY"))
	{
		if(!strncmp(actors[pos].type.c_str(),"std.",4))
		{
			handleTunnelData(msg, reply, request, socket);
			uint8_t moreThanOneMsg = 0;
			uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
			return size;
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
  nextMessage[socket] = 0;
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
	nextMessage[socket] = socketHandler.addToMessagesOut(reply, socket);
#endif
#ifdef _MOCK_
	nextMessage[socket]++;
#endif
	return nextMessage[socket];
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
  for(int i = 0; i < CALIBRATION_TIME; i++)
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
	socketHandler.setupConnection(mac, ip);
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
					if(socketHandler.getSocketConnectionStatus(i))					// active socket?
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

							/*for(int j = 0; j < NUMBER_OF_SUPPORTED_ACTORS; j++)		// 5: Fire actors
							{*/
								if(strcmp(actors[i].type.c_str(),"empty") && actors[i].ackFlag)
								{
									actors[i].fire(&actors[i]);
								}
							//}
						}
					}
				}
			}

			for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)								// 6: Send outgoing message(s)
			{
				if(socketHandler.getSocketConnectionStatus(i))						// active socket?
				{
					socketHandler.sendAllMsg(i);
				}
			}
		}
		socketHandler.NextSocket();
	}
}
#endif
