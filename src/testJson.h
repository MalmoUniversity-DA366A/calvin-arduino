/*
 * testJson.h
 *
 *      Author: Peter
 */
#include "Arduino.h"
#include "ArduinoJson.h"

class testJson {
public:
  void test(JsonObject &root);
  int checkJson(JsonObject &root);
  String jsonDeserialize(char *temp);
  char* jsonSerialize(const char *str);
  String buildStringFromJsonObject(JsonObject &reply);
private:
};
