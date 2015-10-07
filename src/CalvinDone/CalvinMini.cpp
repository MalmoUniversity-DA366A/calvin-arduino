/*
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
#include <util.h>

//byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress ip(192,168,0,5);
//IPAddress ip(192,168,1,146);
uint16_t slaveport = 5002;
EthernetServer server(slaveport);
EthernetClient client;

const int messageOutLength = 4;
String messageOut[messageOutLength] = {};
int nextMessage = 0;
#endif

actor globalActor;
fifo actorFifo;

/**
 * Current standard out is the lcd screen connected to arduino due
 */
int8_t StdOut(){
	uint8_t inFifo;
	const char* token;
	token = "null";
	inFifo = lengthOfData(globalActor.inportsFifo[0]);
	if(inFifo > 0)
	{
		token = fifoPop(globalActor.inportsFifo[0]);
	}
	return standardOut(token);
}

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
extern "C"{
rStatus actorInit(){
  rStatus allOk = FAIL;

  globalActor.fireActor = &StdOut;
  /*This sets up the fifo for the actor, not sure
   *if it should be done here but for now it works*/
  allOk = initFifo(&actorFifo);
  globalActor.inportsFifo[0] = &actorFifo;

  return allOk;
}
}

/**
 * Create an new actor.
 * @param msg json list
 * @return return 1 if successful.
 */
rStatus CalvinMini::createActor(JsonObject &msg){
	rStatus allOk = FAIL;
	JsonObject &state = msg.get("state");
	JsonObject &name = state.get("actor_state");
	globalActor.type = state.get("actor_type");
	globalActor.name = name.get("name");
	globalActor.id = name.get("id");

	allOk = SUCCESS;
	actorInit();
	return allOk;
}

extern "C"{
/**
 * This Function initiate the fifo must be
 * called prior to using the fifo.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 */
rStatus initFifo(fifo *fif)
{
  fif->size = FIFO_SIZE;
  fif->read = 0;
  fif->write = 0;
  return SUCCESS;
}
/**
 * Used by Add and Pop to determine fifo length.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 * @return Fifo length
 */
int8_t lengthOfData(fifo *fif)
{
  return ((fif->write - fif->read) & (fif->size -1));
}

/**
 * Adds a new element to the fifo
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 * @return returns 0 if the fifo is full
 */
rStatus fifoAdd(fifo *fif, const char* element){

  if(lengthOfData(fif) == (fif->size-1))
  {
    return FAIL;      //fifo full;
  }
  fif->element[fif->write] = element;
  fif->write = (fif->write + 1) & (fif->size - 1);

  return SUCCESS;       //all is well
}

/**
 * Return and removes the oldest element in the fifo.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 * @Return Returns fifo element, returns NULL if fifo is
 * empty.
 */
const char* fifoPop(fifo *fif){

  const char* ret;

  if(lengthOfData(fif) == 0)
  {
    return "Null";    //fifo empty
  }

  ret = fif->element[fif->read];
  fif->read = (fif->read + 1) & (fif->size - 1);

  return ret;
}

}
/**
 * Process an incomming token and add the token data to
 * an actor fifo.
 * @param Token data as a string
 * @return if data vas added to fifo this function returns
 * 1, if something went wrong it returns 0.
 */
rStatus CalvinMini::process(const char* token){
  rStatus allOk;
  allOk = FAIL;
  allOk = fifoAdd(globalActor.inportsFifo[0],token);
  return allOk;
}

/**
 * Function for setting the Json reply back to Calvin-Base when the request message from
 * Calvin-Base is "Token"
 * @param msg is the JsonObject that is message from Calvin-Base
 * @param reply is the JsonObject with the reply message from Calvin-Arduino
 */
void CalvinMini::handleToken(JsonObject &msg, JsonObject &reply)
{
  process(msg.get("token"));
  reply.set("cmd",      "TOKEN_REPLY");
  reply.set("sequencenbr",  msg.get("sequencenbr"));
  reply.set("port_id",    msg.get("port_id"));
  reply.set("peer_port_id",   msg.get("peer_port_id"));
  reply.set("value",      "ACK");
}

/**
 * Method for handle the tunnel data using JSON, JSON is added to the JsonObject reference reply
 * @param &msg JsonObject received from Calvin-Base
 * @param &reply JsonObject that is added to the "reply" list
 *
 * Author: Jesper Hansen
 */
void CalvinMini::handleTunnelData(JsonObject &msg, JsonObject &reply,JsonObject &request )
{
	JsonObject &value = msg.get("value");
	reply.set("to_rt_uuid", 	msg.get("from_rt_uuid"));
	reply.set("from_rt_uuid", 	msg.get("to_rt_uuid"));
	reply.set("cmd", 			"TUNNEL_DATA");
	reply.set("tunnel_id",		"NULL"); // None in python
	reply.set("value",			"foo"); // Look in Calvin-Mini.py
#ifdef ARDUINO
	handleMsg(value,reply,request);
#endif
}

void CalvinMini::handleActorNew(JsonObject &msg, JsonObject &reply)
{
	createActor(msg);

	reply.set("cmd",			"REPLY");
	reply.set("msg_uuid",		msg.get("msg_uuid"));
	reply.set("value",			"ACK");
	reply.set("from_rt_uuid",	"calvin-miniscule");
	reply.set("to_rt_uuid",		msg.get("from_rt_uuid"));
}

/**
 * Handle all different messages
 * @param msg JsonObject
 * @param reply JsonObject
 * @param request JsonObject
 */
int8_t CalvinMini::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request)
{
  if(!strcmp(msg.get("cmd"),"JOIN_REQUEST"))
  {
      // JsonObject for replying a join request
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject &policy = jsonBuffer.createObject();
      handleJoin(msg,reply);
      handleSetupTunnel(msg, request, policy);

      // Print JsonObject and send to Calvin
      #ifdef ARDUINO
      Serial.println("Sending...");
      String replyTemp = stringBuilderJsonObject(reply);
      String requestTemp = stringBuilderJsonObject(request);
      addToMessageOut(replyTemp);
      addToMessageOut(requestTemp);
      #endif
      return 1;
  }
  else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
  {
      // reply object + request object
      #ifdef ARDUINO
      Serial.println("ACTOR_NEW");
      #endif
      return 2;
  }
  else if(!strcmp(msg.get("cmd"),"TUNNEL_DATA"))
  {
      // reply object
      #ifdef ARDUINO
      Serial.println("TUNNEL_DATA");
      #endif
      return 3;
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN"))
  {
      // reply object
      #ifdef ARDUINO
      Serial.println("TOKEN");
      #endif
      return 4;
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
  {
      // reply array
      #ifdef ARDUINO
      Serial.println("TOKEN_REPLY");
      #endif
      return 5;
  }
  else if(!strcmp(msg.get("cmd"),"REPLY"))
  {
      #ifdef ARDUINO
      JsonObject &value = msg["value"];
      if(!strcmp(value.get("status"),"ACK"))
      {
          value.printTo(Serial);
      }
      else
      {
          Serial.println("NACK");
      }
      #endif
      return 6;
  }
  else
  {
      #ifdef ARDUINO
      Serial.println("UNKNOWN CMD");
      #endif
      standardOut("UNKNOWN CMD");
      return 7;
  }
}

#ifdef ARDUINO
/**
 * Adds messages to a global array and
 * creates the array size for sending
 * @param reply String
 */
void CalvinMini::addToMessageOut(String reply)
{
  messageOut[nextMessage] = reply;
  if(nextMessage < messageOutLength)
    nextMessage = nextMessage+1;
}

/**
 * Receive message from calvin base
 * @return String
 */
String CalvinMini::recvMsg()
{
  Serial.println("Reading...");
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  byte data[4];
  int found = 0;
  int count = 0;
  int sizeOfMsg;
  while(!found)
  {
        int size = client.readBytes(temp, MAX_LENGTH);
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

/**
 * Reply message to calvin base
 * @param str char pointer of String
 * @param length size of String
 */
void CalvinMini::sendMsg(const char *str, size_t length)
{
  uint32_t hex[4] = {};
  hex[0] = (length & 0xFF000000);
  hex[1] = (length & 0x00FF0000);
  hex[2] = (length & 0x0000FF00);
  hex[3] = (length & 0x000000FF);

  for(int i = 0; i< 4;i++)
  {
    server.write(hex[i]);
  }
  server.write(str);
}
#endif
/**
 * Create a reply message
 * @param msg JsonObject
 * @param reply JsonObject
 */
void CalvinMini::handleJoin(JsonObject &msg, JsonObject &reply)
{
  reply["cmd"] = "JOIN_REPLAY";
  reply["id"] = "calvin-miniscule";
  reply["sid"] = msg.get("sid");
  reply["serializer"] = "json";
}

/**
 * Method for setting up a tunnel using JSON message back to Calvin-Base,
 * JSON is added to the JsonObject request that is added to the reply list.
 * @param &msg JsonObject received from Calvin-Base
 * @param &request JsonObject that is added to the "reply" list
 * @param &policy JsonObject that is an empty JsonObject
 */
void CalvinMini::handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy)
{
  request["msg_uuid"] = "MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb6"; // Should be a unique id
  request["from_rt_uuid"] = "calvin-miniscule";
  request["to_rt_uuid"] = msg.get("id");
  request["cmd"] = "TUNNEL_NEW";
  request["tunnel_id"] = "fake-tunnel";
  request["policy"] = policy; // Unused
  request["type"] = "token";
}

#ifdef ARDUINO
/**
 * Start a server connection
 */
void CalvinMini::setupServer()
{
  //getIPFromRouter(); // Doesn't work with shield
  Ethernet.begin(mac, ip);
  printIp();
  server.begin();
}

/**
 * Prints the IP-address assigned to the Ethernet shield.
 */
void CalvinMini::printIp()
{
    Serial.println(Ethernet.localIP());
}

/**
 * Assign an IP-address to the Ethernet shield.
 */
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

/**
 * Serializes a String to Json syntax.
 * From this: {"sensor":"gps","time":1351824120}
 * To this: {\"sensor\":\"gps\",\"time\":1351824120}
 * @param str char* pointer
 * @return char* pointer
 */
char* CalvinMini::jsonSerialize(const char *str)
{
  const char *json = str;
  char *temp = new char[256];
  int counter = 0;
  for(int i = 0; json[i] != '\0'; i++)
  {
      if(json[i] == '\"')
      {
          temp[counter] = '\\';
          counter++;
      }
      temp[counter] = json[i];
      counter++;
  }
  temp[counter] = '\0';
  return temp;
}

/**
 * Builds a string from a JsonObject
 * by iterating trough the object
 * @param reply JsonObject
 * @return String
 */
String CalvinMini::stringBuilderJsonObject(JsonObject &reply)
{
  String str = "{";
  unsigned int count = 0;
  for(JsonObject::iterator it=reply.begin(); it!=reply.end();++it)
  {
      str += "\"";
      str += it->key;
      str += "\"";
      str += ":";
      if(it->value.is<JsonArray&>())
      {
          JsonArray &array = it->value.asArray();
          str += "[";
          for(unsigned int i = 0; i < array.size(); i++)
          {
              if(array.get(i).operator String())
              {
                  str += "\"";
                  str += array.get(i).asString();
                  str += "\"";
              }
              else
              {
                  str += array.get(i).as<int>();
              }
              if(i != array.size() - 1)
              {
                  str += ",";
              }
           }
           str += "]";
      }
      else if(it->value.is<JsonObject&>())
        {
          JsonObject &object = it->value.asObject();
          str += "{";
          unsigned int innercount = 0;
          for(JsonObject::iterator it=object.begin(); it!=object.end();++it)
          {
              if(it->value.is<JsonArray&>())
              {
                  JsonArray &array = it->value.asArray();
                  str += "[";
                  for(unsigned int i = 0; i < array.size(); i++)
                  {
                      if(array.get(i).operator String())
                      {
                          str += "\"";
                          str += array.get(i).asString();
                          str += "\"";
                      }
                      else
                      {
                          str += array.get(i).as<int>();
                      }
                      if(i != array.size() - 1)
                      {
                          str += ",";
                      }
               }
               str += "]";
              }
              else if(it->value.operator String())
              {
                  str += "\"";
                  str += it->value.asString();
                  str += "\"";
              }
              else
              {
                  str += it->value.as<int>();
              }
              if(count != (object.size() - 1))
              {
                  str += ",";
              }
              innercount++;
          }
          str += "}";
        }
      else if(it->value.operator String())
      {
          str += "\"";
          str += it->value.asString();
          str += "\"";
      }
      else
      {
          str += it->value.as<int>();
      }
      if(count != (reply.size() - 1))
      {
        str += ",";
      }
      count++;
  }
  str += "}";
  return str;
}

void CalvinMini::loop()
{
  setupServer();
  while(1)
  {
      // 1: Kontrollera anslutna sockets
      client = server.available();
      // 2: Fixa koppling
      if(client) // Wait for client
      {
          // 3: Läs av meddelande
          Serial.println("Connected...");
          String str = recvMsg();

          StaticJsonBuffer<4096> jsonBuffer;
          JsonObject &msg = jsonBuffer.parseObject(str.c_str());
          JsonObject &reply = jsonBuffer.createObject();
          JsonObject &request = jsonBuffer.createObject();

          // 4: Hantera meddelande
          handleMsg(msg, reply, request);

          // 5: Fire Actors

          // 6: Läs av utlistan
          for(int i = 0;i < nextMessage;i++)
          {
              // 7: Skicka utmeddelande
              sendMsg(messageOut[i].c_str(),messageOut[i].length());
              messageOut[i] = "";
          }
          nextMessage = 0;
      }
  }
}
#endif
