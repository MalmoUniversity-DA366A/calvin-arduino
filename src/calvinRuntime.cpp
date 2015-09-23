#include <SPI.h>
#include <Ethernet.h>
#include "calvinRuntime.h"
#include "testJson.h"

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
  getIPFromRouter();
  printIp();
  server.begin();
  testJson *json = new testJson; // delete later ??
  while(true)
  {
      client = server.available();
      if(client) // Wait for client
      {
          Serial.println("Reading...");
          char temp[MAX_LENGTH+1];
          int size = client.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
          temp[size-1] = '\0';  // Null terminate char
          if(size)
          {
              // Jsonobject that holds all values
              StaticJsonBuffer<200> jsonBuffer;
              JsonObject &msg = jsonBuffer.parseObject(json->jsonUnserialize(temp));
              json->checkJson(msg); // Test purpose

              // JsonObject for replying a fixed message
              JsonObject &reply = jsonBuffer.parseObject("");
              handleJoin(msg,reply);
              Serial.println(temp); // Print content for test purpose
              delay(1000);

              // Print JsonObject to a string
              String str = "";
              reply.printTo(str);

              // Serialize Json message from string
              char jsonChar[] = {};
              json->jsonSerialize(jsonChar, str.c_str());

              Serial.println("Sending...");
              server.write(jsonChar); // Replay to calvin base
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
  reply["cmd"] = 'JOIN_REPLAY';
  reply["id"] = 'calvin-miniscule';
  reply["sid"] = *msg["sid"];
  reply["serializer"] = 'json';
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
