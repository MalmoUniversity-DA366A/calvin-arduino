#ifdef ARDUINO
/*
 * testJson.cpp
 *
 *      Author: Peter
 */
#include "testJson.h"
#include "ArduinoJson.h"

/**
 * A test function for parsing a JsonObject
 * @param root JsonObject
 */
void testJson::test(JsonObject &root)
{
	root["sensor"] = "gps";
	root["time"] = 1351824120;
	JsonArray &data = root.createNestedArray("data");
	data.add(48.756080, 6);
	data.add(2.302038, 6);

	Serial.println();
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
 * Deserialize Json to a String.
 * From this: {\"sensor\":\"gps\",\"time\":1351824120}
 * To this: {"sensor":"gps","time":1351824120}
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
 * From this: {"sensor":"gps","time":1351824120}
 * To this: {\"sensor\":\"gps\",\"time\":1351824120}
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
 * Builds a string from a JsonObject
 * by iterating trough the object
 * @param reply JsonObject
 * @return String
 */
String testJson::stringBuilderJsonObject(JsonObject &reply)
{
  String str = "{";
  unsigned int count = 0;
  for(JsonObject::iterator it=reply.begin(); it!=reply.end();++it)
  {
      str += "\"";
      str += it->key;
      str += "\"";
      str += ":";
      if(it->value.operator int())
      {
        str += it->value.as<int>();
      }
      else if(it->value.is<JsonArray&>())
      {
          JsonArray &array = it->value.asArray();
          str += "[";
          for(int i = 0; i < array.size(); i++)
          {
              if(array.get(i).operator int())
              {
                  str += array.get(i).as<int>();
              }
              else
              {
                  str += "\"";
                  str += array.get(i).asString();
                  str += "\"";
              }
              if(i != array.size() - 1)
              {
                  str += ",";
              }
          }
          str += "]";
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
      }
      count++;
  }
  str += "}";
  return str;
}
#endif
