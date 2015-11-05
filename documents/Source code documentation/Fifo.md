#Fifo Referens
##Includes
Includes CalvinMini.h

##Functions

rStatus **initFifo** (fifo*)

rStatus **fifoAdd** (fifo*, uint32_t)

uint32_t **fifoPop** (fifo*)

int8_t **lengthOfData** (fifo*)

##Function Documention

 	This Function initiate the fifo must be
 	called prior to using the fifo.
 
###rStatus initFifo(fifo*)

	Adds a new element to the fifo
 	@return returns 0 if the fifo is full

###rStatus fifoAdd(fifo*, uint32_t)


 	Return and removes the oldest element in the fifo.
 	@Return Returns fifo element, returns 1 if fifo is
	empty.

###uint32_t fifoPop(fifo*)

	Used by Add and Pop to determine fifo length.
	@return Fifo length

###int8_t lengthOfData(fifo*)

