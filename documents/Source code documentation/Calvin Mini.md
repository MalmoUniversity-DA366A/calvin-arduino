# Calvin Mini

Classes
------------------------
struct **buffert**

struct **actors**

class **Calvin Mini**

Typedefs
------------------------
typedef unsigned _char_ **BYTE**

typedef struct _buffert_ **fifo**

typedef struct _actors_ **actor**

Enumerations
------------------------
enum _rStatus_ **{ SUCCESS, FAIL }**

Functions
------------------------
rStatus **actorInit (actor \*)**

int8_t **lengthOfData (fifo \*)**

int32_t **fifoPop (fifo \*)**

rStatus **fifoAdd (fifo \***, uint32_t)

rStatus **initFifo (fifo \*)**

Typedef Documentation
------------------------
### typedef struct _buffert_ **fifo**

This is the buffert for a actor. To use an actors port fifo
a buffert struct must be created and assigned to the actor
port fifo. Before this fifo can be used it must be initiated
by the initFifo function.

### typedef struct _actors_ **actor**

This struct contains actor data. References to the actorsfifo is stored in a array of pointers to fifo bufferts. The actor can have multiple fifos on outports and inports, in order to use the they must first be assigned a buffert reference. To execute an actor the actor fire function must be called. Prior to this a reference to the fire function must assigned to the actor. This depends on the actor type. Currently supported actors are: **io.actorStandardOutio**, **io.actorCounter**.

Enumeration Documentation
--------------------------
### enum _rStatus_ **{ SUCCESS, FAIL }**

Enumerator for testing functions SUCCESS indicates operation success and FAIL for operation faield.

Public Member Functions
--------------------------
rStatus **createActor**(JsonObject &msg, uint8-_t socket)

rStatus **process**(uint32_t token, uint8-_t socket)

void **handleToken**(JsonObject &msg, JsonObject &reply, uint8_t socket)

void **sendToken**(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket, uint8_t nextSequenceNbr)

int8_t **lengthOfData**(**fifo \***)

void **handleJoin**(JsonObject &msg, JsonObject &reply, uint8_t socket)

void **handleSetupTunnel**(JsonObject &msg, JsonObject &request, &policy)

void **handleTunnelData**(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket)

int8\_t **handleMsg**(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8\_t socket)

void **loop**()

void **handleActorNew**(JsonObject &msg, JsonObject &reply, uint8_t socket)

void **handleSetupPorts**(JsonObject &msg, JsonObject &request, uint8_t socket)

void **initActorList**()

int8\_t **getActorPos**(const char \*, **actor** \*list)

uint8\_t **addToMessageOut**(String reply, uint8\_t socket)

uint8\_t **packMsg**(JsonObject &msg, JsonObject &request, uint8\_t moreThanOneMsg, uint8\_t socket)

void **calibrateSensor**()

Member Function Documentation
------------------------------
	Create an new actor.
	@param msg json list
	@return return 1 if successful.
### **_rStatus_** **createActor**(JsonObject &msg, uint8-_t socket)
	 
  	Process an incomming token and add the token data to an actor fifo.
   	@param Token data as a string
   	@return if data vas added to fifo this function returns 1, if something went wrong it returns 0.
### **_rStatus_** **createActor**(JsonObject &msg, uint8-_t socket)

	Function for setting the Json reply back to Calvin-Base when the request message from
 	Calvin-Base is "Token". Tokens are recived as int's but the fifo only handles 
 	strings, so they are converted before they are sent off to process. Tokens can
   	only be 16 characters long!. If the token fifo is full a NACK will be returned,
   	token has to be sent one more time.
   	@param msg is the JsonObject that is message from Calvin-Base
   	@param reply is the JsonObject with the reply message from Calvin-Arduino
### void **handleToken**(JsonObject &msg, JsonObject &reply, uint8_t socket)   	
	Function for set values to Json reply. Json reply sends back to Calvin-Base when the
  	request message from Calvin-Base is "TOKEN_REPLY", and when Calvin-Base also sends an
	ACK that indicates that Calvin-Base is ready to receive a new Token. Replys "value" contains an JsonObject
  	that holds the keyword "TOKEN" and the the data at the first position on the FIFO back to Calvin-Base.
 	@param msg is JsonObject that is the message from Calvin-Base
  	@param reply is the JsonObject with the reply message from Calvin-Arduino
  	@param request is the JsonObject that is the nested JsonObject in the reply
  	@param socket is the socket which the token is to be sent to
  	@param nextSequensNbr flags if an ACK or NACK was received. 1 for ACK, 0 for NACK
### void **sendToken**(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket, uint8_t nextSequenceNbr)
  	
	This function is used to determine the length of FIFO
  	@param fif Reference to FIFO buffer
### int8_t **lengthOfData**(**fifo \***)

  	Create a reply message for handle a join
  	@param msg JsonObject
  	@param reply JsonObject
  	@param socket is the socket which is trying to join
### void **handleJoin**(JsonObject &msg, JsonObject &reply, uint8_t socket)
  	
  	Method for setting up a tunnel using JSON message back to Calvin-Base,
  	JSON is added to the JsonObject request that is added to the reply list.
  	@param &msg JsonObject received from Calvin-Base
  	@param &request JsonObject that is added to the "reply" list
	@param &policy JsonObject that is an empty JsonObject
### void **handleSetupTunnel**(JsonObject &msg, JsonObject &request, &policy)
	
   	Function for handle the tunnel data using JSON, JSON is added to the JsonObject reference reply
  	@param &msg JsonObject received from Calvin-Base
  	@param &reply JsonObject that is added to the "reply" list
  	@param socket is the socket which the tunnel belongs to
### void **handleTunnelData**(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8_t socket)
  	
  	Handle all different messages
  	@param msg JsonObject
  	@param reply JsonObject
  	@param request JsonObject
  	@param socket is the socket which the message belongs to
### int8\_t **handleMsg**(JsonObject &msg, JsonObject &reply, JsonObject &request, uint8\_t socket)
  	
	The main loop for Calvin Arduino
### void **loop**()
	
   	Function for handle a new Actor
  	@param msg is the JsonObject that is message from Calvin-Base
  	@param reply is the JsonObject with the reply message from Calvin-Arduino
  	@param socket is the socket which the actor belongs to
### void **handleActorNew**(JsonObject &msg, JsonObject &reply, uint8_t socket)
  	
  	Setup ports. The current version of calvin arduino only uses
  	one port, so this function is mostly present to please calvin base.
  	@param msg input message
  	@param reply Calvin base reply list
  	@param request Calvin base reply list
  	@param socket is the socket which the ports belongs to
### void **handleSetupPorts**(JsonObject &msg, JsonObject &request, uint8_t socket)
  	
  	In current implementation the actorlist has to be initiated with empty actors.
### void **initActorList**()
  	
  	This function returns the position of a specified actor
  	int the actorlist.
  	Currently supported actors are:
  	io.StandardOut
  	std.Counter
  	@param const char* should be the name of the actor needed.
  	@param actor *list pointer to the actor list.
  	@return if the actor is found in the list the return value
  	will be the actor position in the list. Returns -1 if the actor isn't found in the list.
### int8\_t **getActorPos**(const char \*, **actor** \*list)
  	
  	Adds messages to a global array and
 	creates the array size for sending
  	@param reply String
  	@return uint8_t Number of Messages
  	@param socket is the socket which the message is to be sent to
### uint8\_t **addToMessageOut**(String reply, uint8\_t socket)
  	
  	Creates an outmessage to Calvin base
  	@param reply JsonObject
  	@param request JsonObject
  	@param moreThanOneMsg Returns two messages if 1
  	@param socket is the socket which the message is to be sent to
  	@return uint8_t Number of Messages
### uint8\_t **packMsg**(JsonObject &msg, JsonObject &request, uint8\_t moreThanOneMsg, uint8\_t socket)  	

	Calibrates the movement sensor
### void **calibrateSensor**()


 






