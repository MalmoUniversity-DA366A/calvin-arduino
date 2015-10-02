#ifdef ARDUINO
/*
 * calvinRuntime.cpp
 *
 *      Author: Peter
 */
#include <SPI.h>
#include <Ethernet.h>
#include "calvinRuntime.h"
#include "testJson.h"
#include <socket.h>

//byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93 };
//IPAddress ip(192,168,0,5);
IPAddress ip(192,168,1,146);
uint16_t slaveport = 5002;
EthernetServer server(slaveport);
EthernetClient client;
testJson json;

/**
 * Setup TCP connection and receive
 * Json message from calvin base
 */
void calvinRuntime::setupConnection()
{
  //getIPFromRouter(); // Doesn't work with shield
  Ethernet.begin(mac, ip);
  //printIp(); // Test purpose
  server.begin();
  while(true)
  {
      client = server.available();
      if(client) // Wait for client
      {
          String str = recvMsg();

          // Jsonobject that holds all values
          StaticJsonBuffer<2000> jsonBufferMsg;
          JsonObject &msg = jsonBufferMsg.parseObject(str.c_str());
          json.checkJson(msg); // Test purpose

          // JsonObject for replying a fixed message
          StaticJsonBuffer<2000> jsonBufferReply;
          JsonObject &reply = jsonBufferReply.createObject();
          handleMsg(msg, reply);
          json.checkJson(reply); // Test purpose

          // Print JsonObject to a string
          String temp = json.stringBuilderJsonObject(reply);
          Serial.println(temp); // Test purpose

          // Serialize Json message and replay to calvin base
          char *replyJson = json.jsonSerialize(temp.c_str());
          Serial.println(replyJson); // Test purpose
          sendMsg(replyJson);
          delete[] replyJson;

          while(true)
          {
              int i;
              i++;
              Serial.println("In loop");
              delay(500);
          }
      }
  }
}

/**
 * Receive message from calvin base
 * @return String
 */
String calvinRuntime::recvMsg()
{
  Serial.println("Reading..."); // Test purpose
  char temp[MAX_LENGTH+1] = {};
  String str = "";
  bool found = false;
  while(!found)
  {
        int size = client.readBytes(temp, MAX_LENGTH);
        if(*temp == '{')
        {
            temp[size] = '\0';  // Null terminate char
            str += temp;
            found = true;
        }
  }
  while(*temp != '}')
  {
        int size = client.readBytes(temp, MAX_LENGTH);
        temp[size] = '\0';  // Null terminate char
        str += temp;
  }
  return str;
}

/**
 * Reply message to calvin base
 * @param str char pointer
 */
void calvinRuntime::sendMsg(const char *str)
{
  Serial.println("Sending..."); // Test purpose
  char *jsonChar = json.jsonSerialize(str);
  server.write(jsonChar);
  delete[] jsonChar;
  jsonChar = 0;
}

/**
 * Create a reply message
 * @param msg JsonObject
 * @param reply JsonObject
 */
void calvinRuntime::handleJoin(JsonObject &msg, JsonObject &reply)
{
  reply["cmd"] = "JOIN_REPLAY";
  reply["id"] = "calvin-miniscule";
  reply["sid"] = (const char*)msg["sid"];
  reply["serializer"] = "json";
}

/**
 * Handle all different messages
 * @param msg JsonObject
 * @param reply JsonObject
 */
void calvinRuntime::handleMsg(JsonObject &msg, JsonObject &reply)
{
  if(strcmp((const char*)msg["cmd"],"JOIN_REQUEST") == 0)
  {
      handleJoin(msg,reply);
  }
  else if(strcmp((const char*)msg["cmd"],"ACTOR_NEW") == 0)
  {

  }
  else if(strcmp((const char*)msg["cmd"],"TUNNEL_DATA") == 0)
  {

  }
  else if(strcmp((const char*)msg["cmd"],"TOKEN") == 0)
  {

  }
  else if(strcmp((const char*)msg["cmd"],"TOKEN_REPLY") == 0)
  {

  }
  else if(strcmp((const char*)msg["cmd"],"REPLY") == 0)
  {

  }
  else
  {
      Serial.println("UNKNOWN CMD");
  }
}

/**
 * Prints the IP-address assigned to the Ethernet shield.
 */
void calvinRuntime::printIp()
{
    Serial.println(Ethernet.localIP());
}

/**
 * Assign an IP-address to the Ethernet shield.
 */
void calvinRuntime::getIPFromRouter()
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
#endif
