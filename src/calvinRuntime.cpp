#include <SPI.h>
#include <Ethernet.h>
#include "calvinRuntime.h"
#include "testJson.h"
#include <socket.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
byte gateway[] = { 192,168,1,1 };
byte subnet[] = { 255,255,255,0 };
IPAddress ip(192,168,1,202);
uint16_t slaveport = 5001;
EthernetServer server(slaveport);
EthernetClient client;
String messages_in[] = {};
String messages_out[] = {};

/**
 * Setup TCP connection and
 * test Json serialize and unserialize
 * functions from calvin base
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
          Serial.println("Reading..."); // Test purpose
          char temp[MAX_LENGTH+1];
          int size = client.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
          temp[size-1] = '\0';  // Null terminate char
          if(size)
          {
              Serial.println(temp); // Print content for test purpose
              // Jsonobject that holds all values
              StaticJsonBuffer<200> jsonBuffer;
              JsonObject &msg = jsonBuffer.parseObject(temp);
              json.checkJson(msg); // Test purpose

              // JsonObject for replying a fixed message
              StaticJsonBuffer<200> jsonBuffer2;
              JsonObject &reply = jsonBuffer2.createObject();
              handleJoin(msg,reply);

              // Print JsonObject to a string
              String str = json.createStringFromObject(reply);

              // Serialize Json message from string
              char *jsonChar = json.jsonSerialize(str.c_str());
              delay(1000);

              // Replay to calvin base
              Serial.println("Sending..."); // Test purpose
              server.write(jsonChar);
              delete[] jsonChar;
              jsonChar = 0;
              delay(1000);
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
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // Set static IP-address if fail
        Ethernet.begin(mac, ip, gateway, gateway, subnet);
    }
}
