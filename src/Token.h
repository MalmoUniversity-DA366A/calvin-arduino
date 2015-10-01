/*
 * Token.h
 *
 * Author: Jesper Hansen
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include "ArduinoJson.h"

class Token {
public:
	void handleToken(JsonObject &msg, JsonObject &reply);
};

#endif /* TOKEN_H_ */
