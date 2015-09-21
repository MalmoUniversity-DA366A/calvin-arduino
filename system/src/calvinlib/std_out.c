/*
 * std_out.c
 *
 *  Created on: 21 sep. 2015
 *      Author: Daniel Nordahl
 */
#include <stdio.h>
#include "std_out.h"

#ifdef __cplusplus
 extern "C" {
#endif


/**
 * Actor standard out. Default standard out is
 * a display on the arduino board.
 */
int actorStdout(char input[]){

	return printf(input);
}


#ifdef __cplusplus
}
#endif




