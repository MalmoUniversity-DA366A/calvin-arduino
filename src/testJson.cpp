#include "testJson.h"
#include "ArduinoJson.h"

/**
 * A test function for parsing a JsonObject
 * @return JsonObject
 */
JsonObject& testJson::test()
{
	StaticJsonBuffer<200> jsonBuffer;
	char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
	JsonObject &root = jsonBuffer.parseObject(json);
	if (checkJson(root))
	{
	    const char* sensor = root["sensor"];
	    long time = root["time"];
	    double latitude = root["data"][0];
	    double longitude = root["data"][1];
	    Serial.println(sensor);
	    Serial.println(time);
	    Serial.println(latitude, 6); // Number of decimals (6)
	    Serial.println(longitude, 6);
	}
	return root;
}

/**
 * A test function for checking successful parsing
 * @param root JsonObject
 * @return int
 */
int testJson::checkJson(JsonObject &root)
{
	if(!root.success())
	{
	    Serial.println("parseObject() failed");
	    return 0;
	}
	else
	{
	    Serial.println("parseObject() success");
	    return 1;
	}
}

/**
 * Unserializes Json to a String.
 * From this: {\"sensor\":\"gps\",\"time\":\"flies\"}
 * To this: {"sensor":"gps","time":"flies"}
 * @param temp char* pointer
 * @return String
 */
String testJson::jsonDeserialize(char *temp)
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
 * From this: {"sensor":"gps","time":"flies"}
 * To this: {\"sensor\":\"gps\",\"time\":\"flies\"}
 * @param str char* pointer
 * @return char* pointer
 */
char* testJson::jsonSerialize(const char *str)
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
 * Creates a string from a JsonObject
 * by iterating trough the object
 * @param reply
 * @return String
 */
String testJson::createStringFromObject(JsonObject &reply)
{
  String str = "{\"";
  unsigned int count = 0;
  for(JsonObject::iterator it=reply.begin(); it!=reply.end();++it)
  {
      str += it->key;
      str += "\"";
      str += ":";
      if(it->value.operator int())
      {
        str += it->value.as<int>();
      }
      else
      {
        str += "\"";
        str += it->value.asString();
        str += "\"";
      }
      if(count != (reply.size() - 1))
      {
        str += ",";
        str += "\"";
      }
      count++;
  }
  str += "}";
  return str;
}
