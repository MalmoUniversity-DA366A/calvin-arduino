#include "testJson.h"
#include "ArduinoJson.h"

void testJson::test()
{
	StaticJsonBuffer<200> jsonBuffer;
	char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
	JsonObject &root = jsonBuffer.parseObject(json);
	if (!root.success())
	{
	    Serial.println("parseObject() failed");
	}
	const char* sensor = root["sensor"];
	long time = root["time"];
	double latitude = root["data"][0];
	double longitude = root["data"][1];
	Serial.println(sensor);
	Serial.println(time);
	Serial.println(latitude, 6); // Number of decimals (6)
	Serial.println(longitude, 6);
}

void testJson::checkJson(JsonObject &root)
{ // For test purpose
	if(!root.success())
	{
		Serial.println("parseObject() failed");
	}
	else
	{
	    Serial.println("parseObject() success");
	}
	root.printTo(Serial);
	Serial.println();
}

/**
 * Unserializes Json to a String.
 * From this: {\"sensor\":\"gps\",\"time\":\"flies\"}
 * To this: {"sensor":"gps","time":"flies"}
 * @param temp
 * @return str
 */
String testJson::jsonUnserialize(char *temp)
{                     // From this: {\"sensor\":\"gps\",\"time\":\"flies\"}
	String str = "";    // To this: {"sensor":"gps","time":"flies"}
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
 * Serializes a String to Json.
 * From this: {"sensor":"gps","time":"flies"}
 * To this: {\"sensor\":\"gps\",\"time\":\"flies\"}
 * @param temp
 * @param str
 */
void testJson::jsonSerialize(char* temp, const char *str)
{
  int count = 0;
  while(str[count] != '\0')
  {
      if(str[count] == '\"')
      {
          temp[count] = '\\';
      }
      temp[count] = str[count];
      count++;
    }
}
