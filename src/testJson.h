#include "Arduino.h"
#include "ArduinoJson.h"

class testJson {
public:
  void test();
  void checkJson(JsonObject &root);
  String jsonUnserialize(char *temp);
  String jsonSerialize(char *str);
private:
};
