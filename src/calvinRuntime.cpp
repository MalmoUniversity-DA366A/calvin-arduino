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

void calvinRuntime::setupConnection()
{
  getIPFromRouter();
  printIp();
  server.begin();
  testJson json = new testJson; // delete later ??
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
              json.loadJson(json.jsonUnserialize(temp));
              Serial.println(temp); // Print content for test purpose
              delay(1000);

              Serial.println("Sending...");
              server.write("ACK"); // Replay to base
              delay(1000);
          }
      }
  }
}

/**
 * Test Json serialize and unserialize
 * functions from user terminal input
 */
String calvinRuntime::jsonToString(void)
{
  testJson json = new testJson;
  EthernetClient client;
  char temp[MAX_LENGTH+1]; // Make room for NULL terminator
  String str = "";

  int size = client.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
  temp[size-1] = '\0';
  Serial.println(temp); // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
  if(size)              // or      {"sensor":"gps","time":"flies"}
  {
          // Json to String
      str = json.jsonUnserialize(temp);
      json.loadJson(str);     // Prints: {"sensor":"gps","time":"flies"}

          // String to Json
      //str = jsonSerialize(temp);
      //Serial.println(str);  // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
   }
  return str;
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
