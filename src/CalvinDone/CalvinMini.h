#ifndef CALVINDONE_CALVINMINI_H_
#define CALVINDONE_CALVINMINI_H_
#include <stdio.h>
#include <string>
#include "ArduinoJson.h"

#define MAX_LENGTH 									1
#define standardOut(x)    							strlen(x)
#define ACTOR_SIZE      							5
#define QUEUE_SIZE      							10
#define FIFO_SIZE     								8     //Must be a power of two
#define NUMBER_OF_PORTS     						2
#define NUMBER_OF_SUPPORTED_ACTORS					2
#define RT_ID "calvin-arduino"
#define tunnel_id "fake-tunnel"
typedef unsigned char BYTE;

extern "C"{
/*
 * Enum for testing functions SUCCESS indicates
 * operation success and FAIL for operation faield.
 */
typedef enum{
  SUCCESS,
  FAIL
}rStatus;

/**
 * This is the buffert for a actor. To use an actors port fifo
 * a buffert struct must be created and assigned to the actor
 * port fifo. Before this fifo can be used it must be initiated
 * by the initFifo function.
 */
typedef struct buffert{
  uint32_t element[FIFO_SIZE];
  int size;
  int read;
  int write;
}fifo;

/**
 * This struct contains actor data. References to the actors
 * fifo is stored in a array of pointers to fifo bufferts.
 * The actor can have multiple fifos on outports and inports,
 * in order to use the they must first be assigned a buffert
 * reference. To execute an actor the actor fire function must
 * be called. Prior to this a reference to the fire function must
 * assigned to the actor. This depends on the actor type.
 * Currently supported actors are:
 * io.actorStandardOut
 * io.actorCounter
 */
typedef struct actors{
  String type;
  const char* name;
  const char* id;
  String peer_port_id;
  String port_id;
  uint32_t count;
  int8_t (*fireActor)(struct actors*);
  struct buffert inportsFifo[NUMBER_OF_PORTS];
  struct buffert outportsFifo[NUMBER_OF_PORTS];
}actor;

/**
 * This Function initiate the fifo must be
 * called prior to using the fifo.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 */
rStatus initFifo(fifo*);

/**
 * Adds a new element to the fifo
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 *
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 * @return returns 0 if the fifo is full
 */
rStatus fifoAdd(fifo*, uint32_t);

/**
 * Return and removes the oldest element in the fifo.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 * @Return Returns fifo element, returns NULL if fifo is
 * empty.
 */
uint32_t fifoPop(fifo*);

/**
 * Used by Add and Pop to determine fifo length.
 *
 * This fifo implementation is based upon a circular
 * buffert written by Elcia White found in the book
 * "Making Embedded Systems by Elecia White(O'Reilly).
 * Copyright 2012 Elecia White,978-1-449-30214-6"
 *
 * @return Fifo length
 */
int8_t lengthOfData(fifo*);

/**
 *  What's up with the external C you might wonder,
 *  well thats the only way i could ad a function pointer to a strut,
 *  Apparently c++ handles this different from c.
 */
rStatus actorInit(actor*);
rStatus actorInitTest();

/**
 * Current standard out is the lcd screen connected to arduino due
 */
int8_t actorStdOut(actor*);

/**
 * Increment the count each time the actor fires
 */
int8_t actorCount(actor*);

}

using namespace std;
class CalvinMini
{
public:
	CalvinMini(void);
	/**
	 * Create an new actor.
	 * @param msg json list
	 * @return return 1 if successful.
	 */
	rStatus createActor(JsonObject &msg);

  /**
   * Process an incomming token and add the token data to
   * an actor fifo.
   * @param Token data as a string
   * @return if data vas added to fifo this function returns
   * 1, if something went wrong it returns 0.
   */
  rStatus process(uint32_t);

  /**
   * Function for setting the Json reply back to Calvin-Base when the request message from
   * Calvin-Base is "Token". Tokens are recived as int's but the fifo only handles
   * strings, so they are converted before they are sent off to process. Tokens can
   * only be 16 characters long!. If the token fifo is full a NACK will be returned,
   * token has to be sent one more time.
   * @param msg is the JsonObject that is message from Calvin-Base
   * @param reply is the JsonObject with the reply message from Calvin-Arduino
   */
  void handleToken(JsonObject &msg, JsonObject &reply);

  /**
   * Function for set values to Json reply. Json reply sends back to Calvin-Base when the
   * request message from Calvin-Base is "TOKEN_REPLY", and when Calvin-Base also sends an
   * ACK that indicates that Calvin-Base is ready to receive a new Token. Replys "value" contains an JsonObject
   * that holds the keyword "TOKEN" and the the data at the first position on the FIFO back to Calvin-Base.
   * @param msg is JsonObject that is the message from Calvin-Base
   * @param reply is the JsonObject with the reply message from Calvin-Arduino
   * @param request is the JsonObject that is the nested JsonObject in the reply
   */
  void sendToken(JsonObject &msg, JsonObject &reply, JsonObject &request);

  /**
   * This function is used to determine the length of FIFO
   * @param fif Reference to FIFO buffer
   */
  int8_t lengthOfData(fifo*);

  /**
   * Create a reply message for handle a join
   * @param msg JsonObject
   * @param reply JsonObject
   */
  void handleJoin(JsonObject &msg, JsonObject &reply);

  /**
   * Method for setting up a tunnel using JSON message back to Calvin-Base,
   * JSON is added to the JsonObject request that is added to the reply list.
   * @param &msg JsonObject received from Calvin-Base
   * @param &request JsonObject that is added to the "reply" list
   * @param &policy JsonObject that is an empty JsonObject
   */
  void handleSetupTunnel(JsonObject &msg, JsonObject &request, JsonObject &policy);

  /**
   * Function for handle the tunnel data using JSON, JSON is added to the JsonObject reference reply
   * @param &msg JsonObject received from Calvin-Base
   * @param &reply JsonObject that is added to the "reply" list
   *
   * Author: Jesper Hansen
   */
  void handleTunnelData(JsonObject &msg, JsonObject &reply,JsonObject &request);

  /**
   * Handle all different messages
   * @param msg JsonObject
   * @param reply JsonObject
   * @param request JsonObject
   */
  int8_t handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request);

  /**
   * The main loop for Calvin Arduino
   */
  void loop(void);

  /**
   * Function for handle a new Actor
   * @param msg is the JsonObject that is message from Calvin-Base
   * @param reply is the JsonObject with the reply message from Calvin-Arduino
   */
  void handleActorNew(JsonObject &msg, JsonObject &reply);

  /**
   * Setup ports. The current version of calvin arduino only uses
   * one port, so this function is mostly present to please calvin
   * base.
   * @param msg input message
   * @param reply Calvin base reply list
   * @param request Calvin base reply list
   */
  void handleSetupPorts(JsonObject &msg,JsonObject &request);

  /**
   * Reply message to calvin base
   * @param str char pointer of String
   * @param length size of String
   */
  int sendMsg(const char *str, uint32_t length);

  /**
   * Adds messages to a global array and
   * creates the array size for sending
   * @param reply String
   */
  void addToMessageOut(String reply);
#ifdef ARDUINO
  /**
   * Prints the IP-address assigned to the Ethernet shield.
   */
  void printIp(void);

  /**
   * Assign an IP-address to the Ethernet shield.
   */
  void getIPFromRouter(void);

  /**
   * Start a server connection
   */
  void setupServer(void);

  /**
   * Receive message from calvin base
   * @return String
   */
  String recvMsg(void);
#endif
};


#endif /* CALVINDONE_CALVINMINI_H_ */
