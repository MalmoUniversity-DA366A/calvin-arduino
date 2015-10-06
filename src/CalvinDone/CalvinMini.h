
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

typedef struct buffert{
	const char* element[FIFO_SIZE];
	int size;
	int read;
	int write;
}fifo;

int initFifo(fifo*);
int fifoAdd(fifo*, const char*);
const char* fifoPop(fifo*);
int lengthOfData(fifo*);
int actorInit();
int StdOut(void);

typedef struct testStruct{
	struct buffert *inports[5];
}test;

typedef struct actors{
	const char* type;
	const char* name;
	const char* id;
	const char* fifo;
	int (*fireActor)();
	struct buffert *inportsFifo[NUMBER_OF_PORTS];
	struct buffert *outportsFifo[NUMBER_OF_PORTS];
}actor;

}

using namespace std;
class CalvinMini
{
public:
	uint8_t createActor(JsonObject &msg);
	int createJson(void);
	actor getGlobalStruct(void);
	int actorFire(void);
	int8_t* searchForKeys(actor* act,const char*,const char*,const char*);
	int8_t search(const char*);
	void initGlobalActor(actor*);
	int process(const char*);
	void handleToken(JsonObject &msg, JsonObject &reply);
	int lengthOfData(fifo*);
	void handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy);
	void handleTunnelData(JsonObject &msg, JsonObject &reply);
	int handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, JsonObject &policy);
	void loop();
};




#endif /* CALVINDONE_CALVINMINI_H_ */
