
#ifndef CALVINDONE_CALVINMINI_H_
#define CALVINDONE_CALVINMINI_H_

#include <stdio.h>
#include "ArduinoJson.h"
#define standardOut(x)		strlen(x)
#define ACTOR_SIZE			5
#define QUEUE_SIZE			10
#define FIFO_SIZE			8			//Must be a power of two

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

typedef struct actors{
	const char* type;
	const char* name;
	const char* id;
	const char* inport;
	const char* outport;
	const char* fifo;
	int (*function)();
	struct ports{
		const char* key;
		struct tokens{
			const char* key;
			struct fifos{
				const char* key;
				int length;
				const char* (*pop)(struct buffert*);		//Fifo pop and add
				int (*add)(struct buffert*,const char*);		//functions.
			}value[ACTOR_SIZE];
		}value[ACTOR_SIZE];
	}value[ACTOR_SIZE];
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
	void handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, JsonObject &policy);
	void loop();
};




#endif /* CALVINDONE_CALVINMINI_H_ */
