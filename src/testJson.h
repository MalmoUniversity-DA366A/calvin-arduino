#include "Arduino.h"
#include "ArduinoJson.h"

class testJson {
public:
  JsonObject& test();
  int checkJson(JsonObject &root);
  String jsonDeserialize(char *temp);
  char* jsonSerialize(const char *str);
  String createStringFromObject(JsonObject &reply);
private:
};
