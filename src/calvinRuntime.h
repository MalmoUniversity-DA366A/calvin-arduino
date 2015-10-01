/*
 * calvinRuntime.h
 *
 *      Author: Peter
 */
#define MAX_LENGTH 1
#define TERMINATOR 0x0A // $0A in Terminal.exe
#include "ArduinoJson.h"

class calvinRuntime {
public:
  void setupConnection(void);
  String jsonToString(void);
  void printIp(void);
  void getIPFromRouter(void);
  void handleJoin(JsonObject &msg, JsonObject &reply);
  void handleMsg(JsonObject &msg, JsonObject &reply);
  String recvMsg(void);
  void sendMsg(const char *str);

private:
};
