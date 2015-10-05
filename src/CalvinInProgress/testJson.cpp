#ifdef ARDUINO
/*
 * testJson.cpp
 *
 *      Author: Peter
 */

#include "testJson.h"
#include "ArduinoJson.h"
#include "calvinRuntime.h"

/**
 * A test function for creating,
 * parsing and printing a JsonObject
 */
void testJson::testObject()
{
  calvinRuntime calvin;
  StaticJsonBuffer<2000> jsonBuffer;
  //char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[1024,\"inside\"]}";
  //char json[] = "{\"from_rt_uuid\": \"calvin-miniscule\",\"cmd\": \"TUNNEL_NEW\", \"tunnel_id\": 1c17dc88-63b0-48c2-8930-1df399b4c887, \"to_rt_uuid\": \"str\", \"policy\": {},\"type\":\"token\",\"msg_uuid\":00531ac3-1d2d-454d-964a-7e9573f6ebb6}";
  char json[] = "{\"cmd\": \"JOIN_REQUEST\", \"serializers\": [\"json\"], \"id\": \"1c17dc88-63b0-48c2-8930-1df399b4c887\", \"sid\": \"00531ac3-1d2d-454d-964a-7e9573f6ebb6\"}";
  JsonObject &root = jsonBuffer.parseObject(json);
  checkJson(root);
  String str = calvin.stringBuilderJsonObject(root);
  Serial.println(str);
}

/**
 * A test function for creating
 * and printing a JsonObject
 * @param root JsonObject
 */
void testJson::testParsing(JsonObject &root)
{
	root["sensor"] = "gps";
	root["time"] = 1351824120;
	JsonArray &data = root.createNestedArray("data");
	data.add(48.756080, 6);
	data.add(2.302038, 6);

	Serial.println();
	root.printTo(Serial);
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
#endif
