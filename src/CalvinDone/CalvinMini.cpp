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
#include <util.h>
#include <LiquidCrystal.h>

//byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress ip(192,168,0,5);
//IPAddress ip(192,168,1,146);
uint16_t slaveport = 5002;
EthernetServer server(slaveport);
EthernetClient client;
LiquidCrystal lcdOut(52, 50, 48, 46, 44, 42);

const int messageOutLength = 4;
String messageOut[messageOutLength] = {};
int nextMessage = 0;
#endif

actor globalActor;
fifo actorFifo;

int8_t StdOut(){
  uint8_t inFifo;
  const char* token;
  token = "null";
  inFifo = lengthOfData(globalActor.inportsFifo[0]);
  if(inFifo > 0)
  {
    token = fifoPop(globalActor.inportsFifo[0]);
  }
#ifdef ARDUINO
  lcdOut.clear();
  lcdOut.write(token);
#endif
  return standardOut(token);
}

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

rStatus fifoAdd(fifo *fif, const char* element){

  if(lengthOfData(fif) == (fif->size-1))
  {
    return FAIL;      //fifo full;
  }
  fif->element[fif->write] = element;
  fif->write = (fif->write + 1) & (fif->size - 1);

  return SUCCESS;       //all is well
}

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

rStatus CalvinMini::process(const char* token){
  rStatus allOk;
  allOk = FAIL;
  allOk = fifoAdd(globalActor.inportsFifo[0],token);
  return allOk;
}

void CalvinMini::handleToken(JsonObject &msg, JsonObject &reply)
{
  /*Current version of calvin mini only process strings*/
  char tokenData[16];       //No more that 16 chars!!!!
  rStatus fifoStatus;
  JsonObject &token = msg.get("token");
  sprintf(tokenData,"%d",(uint32_t)token.get("data"));
  fifoStatus = process(tokenData);
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

void CalvinMini::handleTunnelData(JsonObject &msg, JsonObject &reply,JsonObject &request)
{
  JsonObject &value = msg.get("value");
  reply.set("to_rt_uuid",   msg.get("from_rt_uuid"));
  reply.set("from_rt_uuid",   RT_ID);
  reply.set("cmd",      "TUNNEL_DATA");
  reply.set("tunnel_id",    tunnel_id); // None in python
#ifdef ARDUINO
  handleMsg(value,reply,request);
#endif
  reply.set("value",      request);
}

void CalvinMini::handleActorNew(JsonObject &msg, JsonObject &reply)
{
  createActor(msg);

  reply.set("cmd",      "REPLY");
  reply.set("msg_uuid",   msg.get("msg_uuid"));
  reply.set("value",      "ACK");
  reply.set("from_rt_uuid", RT_ID);
  reply.set("to_rt_uuid",   msg.get("from_rt_uuid"));
}

void CalvinMini::handleSetupPorts(JsonObject &msg,JsonObject &request)
{
  JsonObject &token = msg["state"]["actor_state"]["inports"]["token"];
  String port_id = token.get("id");
  JsonObject &inports = msg["state"]["prev_connections"]["inports"];
  JsonArray &array = inports[port_id];
  String peer_port_id = array.get(1);
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
}

int8_t CalvinMini::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request)
{
  char replyTemp[2048] = {};
  char requestTemp[2048] = {};
  if(!strcmp(msg.get("cmd"),"JOIN_REQUEST"))
  {
      // JsonObject for replying a join request
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject &policy = jsonBuffer.createObject();
      handleJoin(msg,reply);
      handleSetupTunnel(msg, request, policy);

      // Print JsonObject and send to Calvin
      #ifdef ARDUINO
      reply.printTo(replyTemp,2048);
      request.printTo(requestTemp,2048);

      String str(replyTemp);
      String str2(requestTemp);
      addToMessageOut(str);
      addToMessageOut(str2);
      lcdOut.clear();
      lcdOut.write("JOIN_REQUEST");
      #endif
      return 1;
  }
  else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
  {
      handleActorNew(msg, reply);
      handleSetupPorts(msg,request);
      #ifdef ARDUINO
      reply.printTo(replyTemp,2048);
      request.printTo(requestTemp,2048);

      String str(replyTemp);
      String str2(requestTemp);
      addToMessageOut(str);
      addToMessageOut(str2);
      lcdOut.clear();
      lcdOut.write("ACTOR_NEW");
      #endif
      return 2;
  }
  else if(!strcmp(msg.get("cmd"),"TUNNEL_DATA"))
  {
      handleTunnelData(msg, reply, request);
      #ifdef ARDUINO
      //lcdOut.clear();
      //lcdOut.write("In Tunnel_Data");
      reply.printTo(replyTemp,2048);
      String str(replyTemp);
      addToMessageOut(str);
      //lcdOut.clear();
      //lcdOut.write("TUNNEL_DATA");
      #endif
      return 3;
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN"))
  {
      handleToken(msg,request);
      #ifdef ARDUINO
      lcdOut.clear();
      lcdOut.write("TOKEN");
      //lcdOut.write("In Token");
      //lcdOut.clear();
      //lcdOut.write("TOKEN");
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
      #ifdef ARDUINO
      JsonObject &value = msg["value"];
      if(!strcmp(value.get("status"),"ACK"))
      {
          String test = value.get("status");
          Serial.println(test.c_str());
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
      standardOut("UNKNOWN CMD");
      return 7;
  }
}

#ifdef ARDUINO
void CalvinMini::addToMessageOut(String reply)
{
  messageOut[nextMessage] = reply;
  if(nextMessage < messageOutLength)
    nextMessage = nextMessage+1;
}

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

int CalvinMini::sendMsg(const char *str, uint32_t length)
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

void CalvinMini::handleJoin(JsonObject &msg, JsonObject &reply)
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
void CalvinMini::setupServer()
{
  //getIPFromRouter(); // Doesn't work with shield
  Ethernet.begin(mac, ip);
  printIp();
  server.begin();
}

void CalvinMini::printIp()
{
    Serial.println(Ethernet.localIP());
}

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
  setupServer();
  while(1)
  {
      // 1: Check connected sockets
      client = server.available();
      // 2: Fix connection
      if(client) // Wait for client
      {
          // 3: Read message
          Serial.println("Connected...");
          String str = recvMsg();

          StaticJsonBuffer<4096> jsonBuffer;
          JsonObject &msg = jsonBuffer.parseObject(str.c_str());
          JsonObject &reply = jsonBuffer.createObject();
          JsonObject &request = jsonBuffer.createObject();

          // 4: Handle message
          handleMsg(msg, reply, request);

          // 5: Fire Actors
          //globalActor.fireActor;
          StdOut();
          // 6: Read outgoing message
          for(int i = 0;i < nextMessage;i++)
          {
              // 7: Send outgoing message
              sendMsg(messageOut[i].c_str(),messageOut[i].length());
              messageOut[i] = "";
          }
          nextMessage = 0;
      }
  }
}
#endif
