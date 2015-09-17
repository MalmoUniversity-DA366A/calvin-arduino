#ifdef ARDUINO
#include "Arduino.h"
#include "blinkLED.h"
#ifdef __cplusplus
 extern "C" {
#endif
	void blinkLED(void)
	{
	  pinMode(13,OUTPUT);
		while(1)	//Run loop
		{
	    	delayMicroseconds(1000000);
	    	digitalWrite(13,HIGH);
	    	delayMicroseconds(1000000);
	    	digitalWrite(13,LOW);
	    }
	}
#ifdef __cplusplus
}
#endif
#endif
