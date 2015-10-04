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
  void handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request);
  void handleSetupTunnel(JsonObject &msg, JsonObject &request);
  String recvMsg(void);
  void sendMsg(const char *str);
  String jsonDeserialize(char *temp);
  char* jsonSerialize(const char *str);
  String stringBuilderJsonObject(JsonObject &reply);
private:
};
