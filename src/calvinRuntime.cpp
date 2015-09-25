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

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192,168,1,202);
IPAddress gateway( 192,168,1,1 );
IPAddress subnet( 255,255,255,0 );
uint16_t slaveport = 5001;
EthernetServer server(slaveport);
EthernetClient client;
String messages_in[] = {};
String messages_out[] = {};
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
  testJson json;
  while(true)
  {
      client = server.available();
      if(client) // Wait for client
      {
          if(socket(s, SnMR::TCP, 5001, 0))
            {
              Serial.println("socket");
            }
          Serial.println("socketStatus");
          Serial.println(socketStatus(s));
          if(listen(s))
            {
              Serial.println("listen");
            }

          Serial.println("Reading..."); // Test purpose
          char temp[MAX_LENGTH+1];
          int size = client.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
          temp[size-1] = '\0';  // Null terminate char
          if(size)
          {
              Serial.println(temp); // Print content for test purpose
              // Jsonobject that holds all values
              StaticJsonBuffer<200> jsonBufferMsg;
              JsonObject &msg = jsonBufferMsg.parseObject(temp);
              json.checkJson(msg); // Test purpose

              // JsonObject for replying a fixed message
              StaticJsonBuffer<200> jsonBufferReply;
              JsonObject &reply = jsonBufferReply.createObject();
              handleJoin(msg,reply);

              // Print JsonObject to a string
              String str = json.buildStringFromJsonObject(reply);

              // Serialize Json message from string
              char *jsonChar = json.jsonSerialize(str.c_str());
              delay(100);

              // Replay to calvin base
              Serial.println("Sending..."); // Test purpose
              server.write(jsonChar);
              delete[] jsonChar;
              jsonChar = 0;
              delay(100);
          }
      }
  }
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
        Ethernet.begin(mac, ip, gateway, gateway, subnet);
    }
}
#endif
