#ifdef _MOCK_
/*
 * setupTunnel.h
 *
 *  Created on: 23 sep. 2015
 *      Author: Jesper Hansen
 */

#ifndef SETUPTUNNEL_H_
#define SETUPTUNNEL_H_

#include "ArduinoJson.h"

class SetupTunnel {
public:
	uint32_t handleSetupTunnel(JsonObject &msg, JsonObject &request);
	uint32_t handleTunnelData(JsonObject &msg, JsonObject &reply);
};

#endif /* SETUPTUNNEL_H_ */
#endif
