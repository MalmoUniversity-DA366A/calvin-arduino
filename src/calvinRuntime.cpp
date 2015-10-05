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

/**
 * Setup TCP connection and receive
 * Json message from calvin base
 */
void calvinRuntime::setupConnection()
{
  //getIPFromRouter(); // Doesn't work with shield
  Ethernet.begin(mac, ip);
  printIp();
  server.begin();
  while(true)
  {
      client = server.available();
      if(client) // Wait for client
      {
          Serial.println("Connected...");
          String str = recvMsg();

          // Jsonobject for recieving a join request
          StaticJsonBuffer<4096> jsonBuffer;
          JsonObject &msg = jsonBuffer.parseObject(str.c_str());

          // JsonObject for replying a join request
          JsonObject &reply = jsonBuffer.createObject();
          JsonObject &request = jsonBuffer.createObject();
          JsonObject &policy = jsonBuffer.createObject();

          handleMsg(msg, reply, request, policy);

          // Test purpose
          testJson json;
          json.checkJson(msg);
          json.checkJson(reply);
          json.checkJson(request);

          // Print JsonObject and send to Calvin
          String replyTemp = stringBuilderJsonObject(reply);
          Serial.println("Sending...");
          sendMsg(replyTemp.c_str());
          String requestTemp = stringBuilderJsonObject(request);
          Serial.println("Sending...");
          sendMsg(requestTemp.c_str());

          String tunnelReply = recvMsg();
      }
  }
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
  char *jsonChar = jsonSerialize(str);
  server.write(jsonChar);
  delete[] jsonChar;
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
 */
void calvinRuntime::handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy)
{
  request["msg_uuid"] = "MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb6"; // Should be a unique id
  request["from_rt_uuid"] = "calvin-miniscule";
  request["to_rt_uuid"] = msg.get("id");
  request["cmd"] = "TUNNEL_NEW";
  request["tunnel_id"] = "fake-tunnel";
  request["type"] = "token";
  //request["policy"] = policy; // Unused
}

/**
 * Handle all different messages
 * @param msg JsonObject
 * @param reply JsonObject
 * @param request JsonObject
 */
void calvinRuntime::handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, JsonObject &policy)
{
  if(!strcmp(msg.get("cmd"),"JOIN_REQUEST"))
  {
      handleJoin(msg,reply);
      handleSetupTunnel(msg, request, policy);
  }
  else if(!strcmp(msg.get("cmd"),"ACTOR_NEW"))
  {

  }
  else if(!strcmp(msg.get("cmd"),"TUNNEL_DATA"))
  {

  }
  else if(!strcmp(msg.get("cmd"),"TOKEN"))
  {

  }
  else if(!strcmp(msg.get("cmd"),"TOKEN_REPLY"))
  {

  }
  else if(!strcmp(msg.get("cmd"),"REPLY"))
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
