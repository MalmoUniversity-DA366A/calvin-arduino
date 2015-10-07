/*
 * calvinRuntime.cpp
 *
 *      Author: Peter
 */
#include <SPI.h>
#include <Ethernet.h>
#include <util.h>
#include "calvinRuntime.h"
#include "testJson.h"

//byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xF5, 0x93 };
IPAddress ip(192,168,0,5);
//IPAddress ip(192,168,1,146);
uint16_t slaveport = 5002;
EthernetServer server(slaveport);
EthernetClient client;
testJson jsonTest;

const int messageOutLength = 4;
String messageOut[messageOutLength] = {};
int nextMessage = 0;

/**
 * Setup TCP connection and receive
 * Json message from calvin base
 */
void calvinRuntime::setupConnection()
{
  setupServer();
  while(true)
  {
      client = server.available();
      if(client) // Wait for client
      {
          Serial.println("Connected...");
          String str = recvMsg();

          StaticJsonBuffer<4096> jsonBuffer;
          JsonObject &msg = jsonBuffer.parseObject(str.c_str());
          JsonObject &reply = jsonBuffer.createObject();
          JsonObject &request = jsonBuffer.createObject();
          handleMsg(msg, reply, request);

          for(int i = 0;i < nextMessage;i++)
          {
              sendMsg(messageOut[i].c_str(),messageOut[i].length());
              messageOut[i] = "";
          }
          nextMessage = 0;
      }
  }
}

/**
 * Adds messages to a global array and
 * creates the array size for sending
 * @param reply String
 */
void calvinRuntime::addToMessageOut(String reply)
{
  messageOut[nextMessage] = reply;
  if(nextMessage < messageOutLength)
    nextMessage = nextMessage+1;
}

/**
 * Receive message from calvin base
 * @return String
 */
String calvinRuntime::recvMsg()
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
void calvinRuntime::sendMsg(const char *str, size_t length)
{
  uint32_t hex[4] = {};
  hex[0] = (length & 0xFF000000);
  hex[1] = (length & 0x00FF0000);
  hex[2] = (length & 0x0000FF00);
  hex[3] = (length & 0x000000FF);

  for(int i = 0; i< 4;i++)
    server.write(hex[i]);
  server.write(str);
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
  reply["sid"] = msg.get("sid");
  reply["serializer"] = "json";
}

/**
 * Create a new tunnel request
 * @param msg JsonObject
 * @param request JsonObject
 * @param policy JsonObject
 */
void calvinRuntime::handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy)
{
  request["msg_uuid"] = "MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb6"; // Should be a unique id
  request["from_rt_uuid"] = "calvin-miniscule";
  request["to_rt_uuid"] = msg.get("id");
  request["cmd"] = "TUNNEL_NEW";
  request["tunnel_id"] = "fake-tunnel";
  request["policy"] = policy; // Unused
  request["type"] = "token";
}

/**
 * Handle all different messages
 * @param msg JsonObject
 * @param reply JsonObject
 * @param request JsonObject
 */
void calvinRuntime::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request)
{
  if(!strcmp(msg.get("cmd"),"JOIN_REQUEST"))
  {
      // JsonObject for replying a join request
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject &policy = jsonBuffer.createObject();
      handleJoin(msg,reply);
      handleSetupTunnel(msg, request, policy);

      // Print JsonObject and send to Calvin
      Serial.println("Sending...");
      String replyTemp = stringBuilderJsonObject(reply);
      String requestTemp = stringBuilderJsonObject(request);

      addToMessageOut(replyTemp);
      addToMessageOut(requestTemp);
  }
  else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
  {
      // reply object + request object
      Serial.println("ACTOR_NEW");
  }
  else if(!strcmp(msg.get("cmd"),"TUNNEL_DATA"))
  {
      // reply object
      Serial.println("TUNNEL_DATA");
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN"))
  {
      // reply object
      Serial.println("TOKEN");
  }
  else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
  {
      // reply array
      Serial.println("TOKEN_REPLY");
  }
  else if(!strcmp(msg.get("cmd"),"REPLY"))
  {
      JsonObject &value = msg["value"];
      if(!strcmp(value.get("status"),"ACK"))
      {
        value.printTo(Serial);
      }
      else
      {
        Serial.println("NACK");
      }
      while(true)
      {
          int i=0;
          i++;
          delay(1000);
      }
  }
  else
  {
      Serial.println("UNKNOWN CMD");
  }
}
/**
 * Start a server connection
 */
void calvinRuntime::setupServer()
{
  //getIPFromRouter(); // Doesn't work with shield
  Ethernet.begin(mac, ip);
  printIp();
  server.begin();
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

/**
 * Deserialize Json to a String.
 * From this: {\"sensor\":\"gps\",\"time\":1351824120}
 * To this: {"sensor":"gps","time":1351824120}
 * @param temp char* pointer
 * @return String
 */
String calvinRuntime::jsonDeserialize(char *temp)
{
  String str = "";
  int count = 0;
  while(temp[count] != '\0')
  {
    if(temp[count+1] == '\"')
    {
      count++;
    }
    str += temp[count];
    count++;
  }
  return str;
}

/**
 * Serializes a String to Json syntax.
 * From this: {"sensor":"gps","time":1351824120}
 * To this: {\"sensor\":\"gps\",\"time\":1351824120}
 * @param str char* pointer
 * @return char* pointer
 */
char* calvinRuntime::jsonSerialize(const char *str)
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
String calvinRuntime::stringBuilderJsonObject(JsonObject &reply)
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
