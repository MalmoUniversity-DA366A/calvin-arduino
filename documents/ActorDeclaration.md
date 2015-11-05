#Actor Declaration

### Common denominator
All actors needs to be able to connect to the internet via TCP/IP. This, by default, is accomplished through an **Ethernet shield** using the **W5100** hardware. At this point there are no additional suport for other ethernet shields. However, one could fairly easy replace the Ethernet library with a more suitable one for other hardware configurations. 

## Source actors

#### io.StandardOut

Simply prints the tokens recieved to the serial terminal and a connected LCD breakout. The LCD is by default connected to PINs:
		
	52, 50, 48, 46, 44, 42.

#### io.MovementStandardOut
Acts as a beeper for the **std.MovementSensor** and makes a sound whenever a movement is detected. The speaker is connected to **PIN 31** on the Arduino. 

#### io.LEDStandardOut
Consists of 3 leds connected to the Arduino. Each led is mapped against a number. Whenever that number is recieved, the corresponding led is turned on. To turn **all** simply send the integer '4'. The leds are connected to the following PINs:

	Red - PIN 22

	Yellow - PIN 24

	Green - PIN 26


##Sink actors

#### std.Counter
Simply increments a number by one and sends the number as token data.

#### std.MovementSensor
Detects movement and sends 1 as token data if movement is detected. Sends 0 as token data if no movement is detected. The [movement sensor](https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/overview) is connected to **PIN 22**.

#### std.RFID
RFID reader that maps three differentMiFare Classic cards's UID to an integer 1, 2 and 3 and sends the number as token data. If the RFID reader reads an unknown UID it sends 4 as token data. The RFID reader shield used in the included example is [Adafruit PN532](http://www.adafruit.com/products/789) with the **I2C** protocol enabled.

#### std.SonicRangeSensor
Detects the distance of an object in front of the range sensor and mapps it to an integer. This integer is sent as token data. [The sonic range sensor](http://e-radionica.com/productdata/HCSR04.pdf) is connected to.

	PIN 23 - Eco/Input
	PIN 30 - Trigger/Input
