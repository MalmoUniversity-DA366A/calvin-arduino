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
IPAddress ip(192,168,1,146);
IPAddress gateway( 192,168,1,1 );
IPAddress subnet( 255,255,255,0 );
uint16_t slaveport = 1337;
EthernetServer server(slaveport);
EthernetClient client;
String messages_in[] = {};
String messages_out[] = {};
testJson json;
SOCKET s;

/**
 * Setup TCP connection and receive
 * Json message from calvin base
 */
void calvinRuntime::setupConnection()
{
  //getIPFromRouter(); // Doesn't work with shield
  //Ethernet.begin(mac, ip, gateway, gateway, subnet);
  Ethernet.begin(mac, ip);
  printIp(); // Test purpose
  server.begin();
  while(true)
  {
      client = server.available();
      if(client) // Wait for client
      {
          /*if(socket(s, SnMR::TCP, 5001, 0))
            {
              Serial.println("socket");
            }*/
          /*Serial.println("socketStatus");
          Serial.println(socketStatus(s));*/
          char temp[MAX_LENGTH+1];
          int size = recvMsg(temp);
          if(size)
          {
              // Jsonobject that holds all values
              StaticJsonBuffer<200> jsonBufferMsg;
              JsonObject &msg = jsonBufferMsg.parseObject(temp);
              json.checkJson(msg); // Test purpose

              // JsonObject for replying a fixed message
              StaticJsonBuffer<200> jsonBufferReply;
              JsonObject &reply = jsonBufferReply.createObject();
              handleMsg(msg, reply);

              // Print JsonObject to a string
              String str = json.stringBuilderJsonObject(reply);

              // Serialize Json message and replay to calvin base
              delay(100);
              sendMsg(str.c_str());
              delay(100);
          }
      }
  }
}

int calvinRuntime::recvMsg(char *temp)
{
  Serial.println("Reading..."); // Test purpose
  int size = client.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
  temp[size-1] = '\0';  // Null terminate char
  Serial.println(temp); // Print content for test purpose
  return size;
}

void calvinRuntime::sendMsg(const char *str)
{
  Serial.println("Sending..."); // Test purpose
  char *jsonChar = json.jsonSerialize(str);
  server.write(jsonChar);
  delete[] jsonChar;
  jsonChar = 0;
}

/**
 * Reply message to calvin base
 */
void calvinRuntime::handleJoin(JsonObject &msg, JsonObject &reply)
{
  reply["cmd"] = "JOIN_REPLAY";
  reply["id"] = "calvin-miniscule";
  reply["sid"] = *msg["sid"];
  reply["serializer"] = "json";
}

void calvinRuntime::handleMsg(JsonObject &msg, JsonObject &reply)
{
  String str = msg["cmd"];
  if(strcmp(str.c_str(),"JOIN_REQUEST") == 0)
  {
      handleJoin(msg,reply);
  }
  if(strcmp(str.c_str(),"ACTOR_NEW") == 0)
  {

  }
  if(strcmp(str.c_str(),"TUNNEL_DATA") == 0)
  {

  }
  if(strcmp(str.c_str(),"TOKEN") == 0)
  {

  }
  if(strcmp(str.c_str(),"TOKEN_REPLY") == 0)
  {

  }
  if(strcmp(str.c_str(),"REPLY") == 0)
  {

  }
  else
  {
      /*jprint(msg, prefix="UNKNOWN CMD")
      return None*/
  }
}

/**
 * Prints the IP-address assigned to the Ethernet shield.
 */
void calvinRuntime::printIp()
{
    Serial.println(Ethernet.localIP());
    Serial.println(Ethernet.gatewayIP());
    Serial.println(Ethernet.dnsServerIP());
    Serial.println(Ethernet.subnetMask());
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
