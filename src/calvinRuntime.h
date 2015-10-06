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
  void handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, JsonObject &policy);
  void handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy);
  String recvMsg(void);
  void sendMsg(const char *str, size_t length);
  String jsonDeserialize(char *temp);
  char* jsonSerialize(const char *str);
  String stringBuilderJsonObject(JsonObject &reply);
private:
};
