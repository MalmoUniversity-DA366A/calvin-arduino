#ifdef ARDUINO
#include "testJson.h"
#include "ArduinoJson.h"

void testJson()
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

void printJson(String str)
{
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(str);
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

String jsonUnserialize(char *temp)
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

String jsonSerialize(char *str) // {\"sensor\":\"gps\",\"time\":\"flies\"}
{                               //  {"sensor":"gps","time":"flies"}
  String temp = "";
  int count = 0;
  while(str[count] != '\0')
  {
      if(str[count] == '\"')
      {
          temp += '\\';
      }
      temp += str[count];
      count++;
    }
  return temp;
}
#endif
