#include <SPI.h>
#include <Ethernet.h>
#include "calvinRuntime.h"
#include "testJson.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);
EthernetServer server = EthernetServer(1337);
EthernetClient client;
String messages_in[] = {};
String messages_out[] = {};

void setupConnection() {
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0)
  {
      Serial.println("Failed to configure Ethernet using DHCP");
      // no point in carrying on, so do nothing forevermore:
      // try to configure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
  }
  printIp();
  server.begin();
  while(true)
  {
      client = server.available();
      if (client == true)
      {
          // read bytes from the incoming client and write them back
          // to any clients connected to the server:
          String rread = jsonToString();
      }
    }
}
String jsonToString(void)
{
  char temp[MAX_LENGTH+1]; // Make room for NULL terminator
  String str = "";

  int size = client.readBytesUntil(TERMINATOR, temp, MAX_LENGTH);
  temp[size-1] = '\0';
  Serial.println(temp); // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
  if(size)              // or      {"sensor":"gps","time":"flies"}
  {
          // Json to String
      str = jsonUnserialize(temp);
      //printJson(str);     // Prints: {"sensor":"gps","time":"flies"}

          // String to Json
      //str = jsonSerialize(temp);
      //Serial.println(str);  // Prints: {\"sensor\":\"gps\",\"time\":\"flies\"}
   }
  return str;
}

/**
 * Prints the IP-address assigned to the Ethernet shield.
 */
void printIp()
{
  Serial.println();
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    // print the value of each byte of the IP address:
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print(".");
    }
    Serial.println();
}
