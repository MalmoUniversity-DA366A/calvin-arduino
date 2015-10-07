/*
 * calvinRuntime.h
 *
 *      Author: Peter
 */
#define MAX_LENGTH 1
#include "ArduinoJson.h"

class calvinRuntime {
public:
  void setupConnection(void);
  String jsonToString(void);
  void printIp(void);
  void getIPFromRouter(void);
  void setupServer(void);
  void handleJoin(JsonObject &msg, JsonObject &reply);
  void handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request);
  void handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy);
  void addToMessageOut(String reply);
  String recvMsg(void);
  void sendMsg(const char *str, size_t length);
  String jsonDeserialize(char *temp);
  char* jsonSerialize(const char *str);
  String stringBuilderJsonObject(JsonObject &reply);
private:
};
