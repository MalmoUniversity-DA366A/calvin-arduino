/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015
 *
 * Andreas Elvstam, Jesper Hansen, Peter Johansson, Daniel Nordahl, Carl �str�m
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CALVINDONE_CALVINMINI_H_
#define CALVINDONE_CALVINMINI_H_
#include <stdio.h>
#include "ArduinoJson.h"
#include <Ethernet.h>
#define MAX_LENGTH 									1
#define standardOut(x)    							strlen(x)
#define ACTOR_SIZE      							5
#define FIFO_SIZE     								8     //Must be a power of two
#define NUMBER_OF_PORTS     						4
#define NUMBER_OF_SUPPORTED_ACTORS					4
#define tunnel_id "fake-tunnel"
// Sensor calibration time (10-60 sec according to the data sheet)
#define CALIBRATION_TIME 							10

typedef unsigned char BYTE;

extern "C"{
/**
 * Enumerator for testing functions SUCCESS indicates
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
  int8_t (*fire)(struct actors*);
  struct buffert inportsFifo[NUMBER_OF_PORTS];
  struct buffert outportsFifo[NUMBER_OF_PORTS];
  uint8_t ackFlag;
}actor;

extern 	rStatus actorInit(actor*);
extern  int8_t lengthOfData(fifo*);
extern	uint32_t fifoPop(fifo*);
extern	rStatus fifoAdd(fifo*, uint32_t);
extern	rStatus initFifo(fifo*);
}

using namespace std;
class CalvinMini
{
public:
	CalvinMini(String rtID, byte* macAdr, IPAddress ipAdr, uint16_t port);
	/**
	 * Create an new actor.
	 * @param msg json list
	 * @return return 1 if successful.
	 */
	rStatus createActor(JsonObject &msg, uint8_t socket);

  /**
   * Process an incomming token and add the token data to
   * an actor fifo.
   * @param Token data as a string
   * @return if data vas added to fifo this function returns
   * 1, if something went wrong it returns 0.
   */
  rStatus process(uint32_t token, uint8_t socket);

  /**
   * Function for setting the Json reply back to Calvin-Base when the request message from
   * Calvin-Base is "Token". Tokens are recived as int's but the fifo only handles
   * strings, so they are converted before they are sent off to process. Tokens can
   * only be 16 characters long!. If the token fifo is full a NACK will be returned,
   * token has to be sent one more time.
   * @param msg is the JsonObject that is message from Calvin-Base
   * @param reply is the JsonObject with the reply message from Calvin-Arduino
   */
  void handleToken(JsonObject &msg, JsonObject &reply, uint8_t socket);

  /**
   * Function for set values to Json reply. Json reply sends back to Calvin-Base when the
   * request message from Calvin-Base is "TOKEN_REPLY", and when Calvin-Base also sends an
   * ACK that indicates that Calvin-Base is ready to receive a new Token. Replys "value" contains an JsonObject
   * that holds the keyword "TOKEN" and the the data at the first position on the FIFO back to Calvin-Base.
   * @param msg is JsonObject that is the message from Calvin-Base
   * @param reply is the JsonObject with the reply message from Calvin-Arduino
   * @param request is the JsonObject that is the nested JsonObject in the reply
   * @param socket is the socket which the token is to be sent to
   * @param nextSequensNbr flags if an ACK or NACK was received. 1 for ACK, 0 for NACK
   */
  void sendToken(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket, uint8_t nextSequenceNbr);

  /**
   * This function is used to determine the length of FIFO
   * @param fif Reference to FIFO buffer
   */
  int8_t lengthOfData(fifo*);

  /**
   * Create a reply message for handle a join
   * @param msg JsonObject
   * @param reply JsonObject
   * @param socket is the socket which is trying to join
   */
  void handleJoin(JsonObject &msg, JsonObject &reply, uint8_t socket);

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
   * @param socket is the socket which the tunnel belongs to
   */
  void handleTunnelData(JsonObject &msg, JsonObject &reply,JsonObject &request, uint8_t socket);

  /**
   * Handle all different messages
   * @param msg JsonObject
   * @param reply JsonObject
   * @param request JsonObject
   * @param socket is the socket which the message belongs to
   */
  int8_t handleMsg(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket);

  /**
   * The main loop for Calvin Arduino
   */
  void loop(void);

  /**
   * Function for handle a new Actor
   * @param msg is the JsonObject that is message from Calvin-Base
   * @param reply is the JsonObject with the reply message from Calvin-Arduino
   * @param socket is the socket which the actor belongs to
   */
  void handleActorNew(JsonObject &msg, JsonObject &reply, uint8_t socket);

  /**
   * Setup ports. The current version of calvin arduino only uses
   * one port, so this function is mostly present to please calvin
   * base.
   * @param msg input message
   * @param reply Calvin base reply list
   * @param request Calvin base reply list
   * @param socket is the socket which the ports belongs to
   */
  void handleSetupPorts(JsonObject &msg,JsonObject &request, uint8_t socket);

  /**
   * In current implementation the actorlist has to be initiated
   * with empty actors.
   */
  void initActorList();

  /**
   * This function returns the position of a specified actor
   * int the actorlist.
   * Currently supported actors are:
   * io.StandardOut
   * std.Counter
   * @param const char* should be the name of the actor needed.
   * @param actor *list pointer to the actor list.
   * @return if the actor is found in the list the return value
   * will be the actor position in the list. Returns -1 if the actor
   * Isn't found in the list.
   */
  int8_t getActorPos(const char*,actor *list);

  /**
   * Adds messages to a global array and
   * creates the array size for sending
   * @param reply String
   * @return uint8_t Number of Messages
   * @param socket is the socket which the message is to be sent to
   */
  uint8_t addToMessageOut(String reply, uint8_t socket);

  /**
   * Creates an outmessage to Calvin base
   * @param reply JsonObject
   * @param request JsonObject
   * @param moreThanOneMsg Returns two messages if 1
   * @param socket is the socket which the message is to be sent to
   * @return uint8_t Number of Messages
   */
  uint8_t packMsg(JsonObject &reply, JsonObject &request, uint8_t moreThanOneMsg, uint8_t socket);

  /**
   * Calibrates movement sensor
   */
  void calibrateSensor(void);

};


#endif /* CALVINDONE_CALVINMINI_H_ */
