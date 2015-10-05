/*
 * testJson.h
 *
 *      Author: Peter
 */
#include "Arduino.h"
#include "ArduinoJson.h"

class testJson {
public:
  void testObject(void);
  void testParsing(JsonObject &root);
  int checkJson(JsonObject &root);
private:
};
