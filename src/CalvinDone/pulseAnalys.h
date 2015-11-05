/*
 * pulseAnalys.h
 *
 *  Created on: 4 nov. 2015
 *      Author: Carl
 */
#ifndef CALVINDONE_PULSEANALYS_H_
#define CALVINDONE_PULSEANALYS_H_
#ifdef ARDUINO
#include "Arduino.h"
extern "C"{

	unsigned long countPulseASM(const volatile uint32_t *port, uint32_t bit, uint32_t stateMask, unsigned long maxloops);
	/*
	 * \brief Measures the length (in microseconds) of a pulse on the pin; state is HIGH
	 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
	 * to 3 minutes in length, but must be called at least a few dozen microseconds
	 * before the start of the pulse.
	 */
	extern uint32_t pulseIn( uint32_t ulPin, uint32_t ulState, uint32_t ulTimeout) ;
}
#endif
#endif /* CALVINDONE_PULSEANALYS_H_*/
