/*
 * setupTunnel.h
 *
 *  Created on: 23 sep. 2015
 *      Author: Jesper
 */

#ifndef SETUPTUNNEL_H_
#define SETUPTUNNEL_H_

#include "ArduinoJson.h"

class setupTunnel {
public:
	int handleSetupTunnel(JsonObject &msg, JsonObject &request);
	int handleTunnelData(JsonObject &msg, JsonObject &reply);
};



#endif /* SETUPTUNNEL_H_ */
