/*
 * pulseAnalys.cpp
 *
 * 		Created on: 4 nov. 2015
 *      Author: Carl
 */

#ifdef ARDUINO
#include "pulseAnalys.h"
#include "Arduino.h"
extern "C"{

uint32_t pulseIn( uint32_t pin, uint32_t state, uint32_t timeout )
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
	PinDescription p = g_APinDescription[pin];
	uint32_t bit = p.ulPin;
	uint32_t stateMask = state ? bit : 0;

	// convert the timeout from microseconds to a number of times through
	// the initial loop; it takes (roughly) 18 clock cycles per iteration.
	uint32_t maxloops = microsecondsToClockCycles(timeout) / 18;

	uint32_t width = countPulseASM(&(p.pPort->PIO_PDSR), bit, stateMask, maxloops);

	// convert the reading to microseconds. The loop has been determined
	// to be 18 clock cycles long and have about 16 clocks between the edge
	// and the start of the loop. There will be some error introduced by
	// the interrupt handlers.
	if (width)
	{
		uint32_t clockCycles = clockCyclesToMicroseconds(width * 18 + 16);
		return clockCycles;
	}
	else
		return 0;
}

unsigned long countPulseASM(const volatile uint32_t *port, uint32_t bit, uint32_t stateMask, unsigned long maxloops)
{
	unsigned long width = 0;

	// wait for any previous pulse to end
	while ((*port & bit) == stateMask)
		if (--maxloops == 0)
			return 0;

	// wait for the pulse to start
	while ((*port & bit) != stateMask)
		if (--maxloops == 0)
			return 0;

	// wait for the pulse to stop
	while ((*port & bit) == stateMask) {
		if (++width == maxloops)
			return 0;
	}
	return width;
}
}
#endif /* ARDUINO */
