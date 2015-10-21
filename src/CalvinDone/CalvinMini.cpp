/**
 * This is the Calvin mini library for Arduino due
 *
 *Created on: 5 okt. 2015
 *
 */
#include <stdio.h>
#include "CalvinMini.h"
#include <inttypes.h>
#include <string>

#ifdef ARDUINO
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include "../CalvinInProgress/handleSockets.h"

//byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress ip(192,168,0,10);
//IPAddress ip(192,168,1,146);
uint16_t slaveport = 5002;
EthernetServer server(slaveport);
EthernetClient client;
LiquidCrystal lcdOut(52, 50, 48, 46, 44, 42);
HandleSockets socketHandler;
#endif

const int messageOutLength = 4;
String messageOut[messageOutLength] = {};
uint8_t nextMessage = 0;

actor globalActor;
fifo actorFifo;
uint32_t sequenceNbr = 0;

int8_t StdOut(){
  uint8_t inFifo;
  char tokenData[16];
  inFifo = lengthOfData(globalActor.inportsFifo[0]);
  if(inFifo > 0)
  {
      sprintf(tokenData,"%d",(uint32_t)fifoPop(globalActor.inportsFifo[0]));
  }
#ifdef ARDUINO
  Serial.println(tokenData);
  lcdOut.clear();
  lcdOut.write(tokenData);
#endif
  return standardOut(tokenData);
}

int8_t actorCount()
{
	int8_t allOk = FAIL;
	uint32_t count;
	char tokenData[16];
	++(globalActor.count);
	count = globalActor.count;
	allOk = fifoAdd(globalActor.inportsFifo[0],count);
  sprintf(tokenData,"%d",(uint32_t)count);
#ifdef ARDUINO
  Serial.println(tokenData);
	lcdOut.clear();
	lcdOut.write(tokenData);
  delay(400); // Stable at 300ms
#endif

	return allOk;
}

extern "C"{
rStatus actorInit(){
  rStatus allOk = FAIL;
#ifdef ARDUINO
  if(!strcmp(globalActor.type.c_str(),"io.StandardOut"))
  {
	  globalActor.fireActor = &StdOut;
  }else
  {
	  globalActor.fireActor = &actorCount;
  }
#else
  globalActor.fireActor = &StdOut;
#endif
  /*This sets up the fifo for the actor, not sure
   *if it should be done here but for now it works*/
 globalActor.inportsFifo[0] = &actorFifo;
  allOk = initFifo(&actorFifo);
  //globalActor.inportsFifo[0] = &actorFifo;

  return allOk;
}
rStatus actorInitTest(){
	rStatus allOk = FAIL;


	globalActor.fireActor = &StdOut;
	/*This sets up the fifo for the actor, not sure
	 *if it should be done here but for now it works*/
	allOk = initFifo(&actorFifo);
	globalActor.inportsFifo[0] = &actorFifo;

	  return allOk;
}
}

rStatus CalvinMini::createActor(JsonObject &msg){
  rStatus allOk = FAIL;
  JsonObject &state = msg.get("state");
  JsonObject &name = state.get("actor_state");
#ifdef ARDUINO
  globalActor.type = state.get("actor_type").asString();
#else
  globalActor.type = state.get("actor_type");
#endif
  globalActor.name = name.get("name");
  globalActor.id = name.get("id");
  globalActor.count = (uint32_t)name.get("count");

  allOk = SUCCESS;
  actorInit();
  return allOk;
}

extern "C"{
rStatus initFifo(fifo *fif)
{
  fif->size = FIFO_SIZE;
  fif->read = 0;
  fif->write = 0;
  return SUCCESS;
}

int8_t lengthOfData(fifo *fif)
{
  return ((fif->write - fif->read) & (fif->size -1));
}

rStatus fifoAdd(fifo *fif, uint32_t element){

  if(lengthOfData(fif) == (fif->size-1))
  {
    return FAIL;      //fifo full;
  }
  fif->element[fif->write] = element;
  fif->write = (fif->write + 1) & (fif->size - 1);

  return SUCCESS;       //all is well
}

uint32_t fifoPop(fifo *fif){

	uint32_t ret;

  if(lengthOfData(fif) == 0)
  {
    return FAIL;    //fifo empty
  }

  ret = fif->element[fif->read];
  fif->read = (fif->read + 1) & (fif->size - 1);

  return ret;
}

}

rStatus CalvinMini::process(uint32_t token){
  rStatus allOk;
  allOk = FAIL;
  allOk = fifoAdd(globalActor.inportsFifo[0],token);
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
  }else
  {
    reply.set("value",      "NACK");  //Fifo is full, please come again
  }
}

void CalvinMini::sendToken(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket)
{
  request.set("data", fifoPop(globalActor.inportsFifo[0]));
	request.set("type", "Token");

	reply.set("sequencenbr", sequenceNbr);
	sequenceNbr++;

	reply.set("token", request);
	reply.set("cmd", "TOKEN");
	reply.set("port_id", globalActor.port_id);
	reply.set("peer_port_id", globalActor.peer_port_id);
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
#ifdef ARDUINO
      while(true)
      {
          lcdOut.clear();
          lcdOut.write("NACK");
      }
#endif
  }
  else if(!strcmp(value.get("cmd"), "TOKEN"))
  {
#ifdef ARDUINO
      handleMsg(value,reply,request, socket);
#endif
  }
  else
  {
      sendToken(value, request, token, socket);
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
  request.set("peer_actor_id", NULL);
  request.set("peer_port_name", NULL);
  request.set("peer_port_dir", NULL);
  request.set("tunnel_id", tunnel_id);
  request.set("cmd", "PORT_CONNECT");
  globalActor.peer_port_id = peer_port_id;
  globalActor.port_id = port_id;
}

int8_t CalvinMini::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket)
{
  char replyTemp[2048] = {};
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
      lcdOut.clear();
      lcdOut.write("JOIN_REQUEST");
      #endif
      return size;
  }
  else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
  {
      handleActorNew(msg, reply, socket);
      handleSetupPorts(msg, request, socket);

      uint8_t moreThanOneMsg = 1;
      uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
      #ifdef ARDUINO
      lcdOut.clear();
      lcdOut.write("ACTOR_NEW");
      #endif
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
      handleToken(msg,request);
      #ifdef ARDUINO
      lcdOut.clear();
      lcdOut.write("TOKEN");
      #endif
      return 4;
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
  {
      // reply array
      return 5;
  }
  else if(!strcmp(msg.get("cmd"),"REPLY"))
  {
      //if(!strcmp(value.get("reply"),"ACK") && !strcmp(globalActor.type.c_str(),"std.Counter"))
      if(!strcmp(globalActor.type.c_str(),"std.Counter"))
      {
        handleTunnelData(msg, reply, request, socket);
        uint8_t moreThanOneMsg = 0;
        uint8_t size = packMsg(reply, request, moreThanOneMsg, socket);
        return size;
      }
      /*JsonObject &value = msg["value"];
      if(!strcmp(value.get("status"),"ACK"))
      {
          Serial.println("ACK");
      }
      else
      {
#ifdef ARDUINO
          Serial.println("NACK");
#endif
      }*/
      return 6;
  }
  else
  {
      standardOut("UNKNOWN CMD");
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
 // messageOut[nextMessage] = reply;
  socketHandler.addToMessagesOut(reply, socket);
  if(nextMessage < messageOutLength)
    nextMessage = nextMessage+1;
  return nextMessage;
}

#ifdef ARDUINO
String CalvinMini::recvMsg()
{
  Serial.println("Reading...");
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  BYTE data[4];
  int found = 0;
  int count = 0;
  int sizeOfMsg;
  while(!found)
  {
        client.readBytes(temp, MAX_LENGTH);
        data[count] = *temp;
        count++;
        if(*temp == '{')
        {
            str += temp;
            found = 1;
        }
  }
  sizeOfMsg = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
  for(int i = 0;i < sizeOfMsg-1;i++)
  {
      int size = client.readBytes(temp, MAX_LENGTH);
      temp[size] = '\0';  // Null terminate char
      str += temp;
  }
  return str;
}
#endif
//-----------kan tas bort--------------------
uint8_t CalvinMini::sendMsg(const char *str, uint32_t length)
{
  BYTE hex[4] = {};
  hex[0] = (length & 0xFF000000);
  hex[1] = (length & 0x00FF0000);
  hex[2] = (length & 0x0000FF00) / 0x000000FF;
  hex[3] = (length & 0x000000FF);
#ifdef ARDUINO
  server.write(hex,4);
  server.write(str);
#endif
  if(length == (hex[2]*256 + hex[3]))
    return 1;
  else
    return 0;
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

//-----------kan tas bort--------------------
#ifdef ARDUINO
void CalvinMini::setupServer()
{
  //getIPFromRouter(); // Doesn't work with shield
  Ethernet.begin(mac, ip);
  printIp();
  server.begin();
}
//-----------kan tas bort--------------------
void CalvinMini::printIp()
{
    Serial.println(Ethernet.localIP());
}
//-----------kan tas bort--------------------
void CalvinMini::getIPFromRouter()
{
    // Disable SD
    pinMode(4,OUTPUT);
    digitalWrite(4,HIGH);
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // Set static IP-address if fail
        Ethernet.begin(mac, ip);
    }
}

void CalvinMini::loop()
{
	lcdOut.write("Hello Calvin");
	globalActor.fireActor = &StdOut;
	socketHandler.setupConnection(mac, ip);
	socketHandler.prepareMessagesOut();
	delay(500);
	while(1)
	{
		// 1: Check connected sockets
		socketHandler.determineSocketStatus();
		// 3: Read message
		socketHandler.recvAllMsg();
		lcdOut.clear();
		// 4: Handle message
		for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)
		{
			const char* message = socketHandler.getMessagesIn(i).c_str();
			if(strncmp(socketHandler.EMPTY_STR, message, 9)!= 0)
			{
				String str = socketHandler.getMessagesIn(i);
				StaticJsonBuffer<4096> jsonBuffer;
				JsonObject &msg = jsonBuffer.parseObject(str.c_str());
				JsonObject &reply = jsonBuffer.createObject();
				JsonObject &request = jsonBuffer.createObject();
				handleMsg(msg, reply, request, i);
			}
		}

		// 5: Fire Actors
		globalActor.fireActor();

		// 6: Send outgoing message
		for(int i = 0; i < MAX_NBR_OF_SOCKETS; i++)
		{
			socketHandler.sendAllMsg(i);
		}

		socketHandler.NextSocket();
	}
}
#endif
