#ifndef CALVINDONE_CALVINMINI_H_
#define CALVINDONE_CALVINMINI_H_

#include <stdio.h>
#include "ArduinoJson.h"
#define standardOut(x)		strlen(x)
#define ACTOR_SIZE			5
#define QUEUE_SIZE			10
#define FIFO_SIZE			8			//Must be a power of two
#define NUMBER_OF_PORTS     2

extern "C"{
/*
 * Enum for testing functions SUCCESS indicates
 * operation success and FAIL for operation faield.
 */
typedef enum{
	SUCCESS,
	FAIL
}rStatus;

typedef struct buffert{
	const char* element[FIFO_SIZE];
	int size;
	int read;
	int write;
}fifo;

typedef struct actors{
	const char* type;
	const char* name;
	const char* id;
	const char* fifo;
	int8_t (*fireActor)();
	struct buffert *inportsFifo[NUMBER_OF_PORTS];
	struct buffert *outportsFifo[NUMBER_OF_PORTS];
}actor;

rStatus initFifo(fifo*);
rStatus fifoAdd(fifo*, const char*);
const char* fifoPop(fifo*);
int8_t lengthOfData(fifo*);
rStatus actorInit();
int8_t StdOut(void);


}

using namespace std;
class CalvinMini
{
public:
	rStatus createActor(JsonObject &msg);
	void initGlobalActor(actor*);
	rStatus process(const char*);
	void handleToken(JsonObject &msg, JsonObject &reply);
	int8_t lengthOfData(fifo*);
	void handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy);
	void handleTunnelData(JsonObject &msg, JsonObject &reply);
	int8_t handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, JsonObject &policy);
	void loop();
};




#endif /* CALVINDONE_CALVINMINI_H_ */
