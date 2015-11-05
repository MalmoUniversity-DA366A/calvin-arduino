#Actors

Classes
-------

class **Actors**

Includes
--------

include "CalvinMini.h"

Functions
---------

rStatus 	**actorInit** (actor *)

int8_t 	**actorStdOut** (actor *)

int8_t 	**actorCount** (actor *)

int8_t 	**actorMovement** (actor *)

int8_t 	**movementStd** (actor *inputActor)

int8_t 	**actorRFID** (actor *)

int8_t    **actorSonicRange** (actor *)

void 	   **setupSonicRange** (void)

uint8_t   **rfidSetup** (void)

uint32_t 	**compareMifareClassicCardUid** (uint8_t *)

uint8_t 	**readRFID** (uint8_t *)

int8_t 	**actorLED** (actor *inputActor)

void 	**controlLed** (uint32_t)

void 	**setupLedOut** (void)

Function Documentation
----------------------

	Increment the count each time the actor fires
### int8_t **actorCount**	(	actor * 		)

	What's up with the external C you might wonder, well 
	thats the only way i could ad a function pointer to a 
	strut, Apparently c++ handles this different from c.
### rStatus **actorInit**	(	actor * 		)	

	Controls three different LED's depending on the input from FiFo 
	@Param uint8_t* uid, the array to store the UID of the read RFID card.
	@Return uint8_t result returns 1 if a classic MiFare RFID card was
	detected, else 0.
### int8_t **actorLED**	(	actor * 	inputActor	)	

	Handles a movement sensor actor
### int8_t **actorMovement**	(	actor * 		)	

	Reads RFID cards and determine the UID of the card.
### int8_t **actorRFID**	(	actor * 		)	

	Sonic range sensor actor, reads the distance and convert it to a 
	number from 0 to 4 for the ledActor.
### int8_t **actorSonicRange**	(	actor * 		)	

	Current standard out is the lcd screen connected to arduino due
	
### int8_t **actorStdOut**	(	actor * 		)

	Determine what card was read. 
	@Param uint8_t* uid, the UID to compare
	@Return uint32_t the value matching the UID. If none found 0.
### uint32_t **compareMifareClassicCardUid**	(	uint8_t * 		)

	Determines which LED to turn ON/OFF 
	@Param uint32_t id the id 
	corresponding to different LED commands
### void **controlLed**	(	uint32_t 		)

	Receives data from a movement actor and sets output as high if
	it receives movement flag.
### int8_t **movementStd**	(	actor * 	inputActor	)

	Reads an RFID card/tag UID. 
	@Param uint8_t* uid, the array to store the 
	read UID. 
	@Return int8_t returns 1 if successful read, else 0.
### uint8_t **readRFID**	(	uint8_t * 		)

	Setup the RFID reader 
	@Return uint8_t 1 if success, else 0.
### uint8_t **rfidSetup**	(	void 		)

	Sets PIN's for the LED's used. Initiate PIN's and set to LOW
### void **setupLedOut**	(	void 		)	

	Setting up Sonic range pins
### void **setupSonicRange**	(	void 		)
