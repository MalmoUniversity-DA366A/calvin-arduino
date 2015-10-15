# Calvin Mini Arduino Library

## Description

The Calvin Arduino library is an implementation of Ericsson's Calvin minimal. Calvin Arduino enables you to run a Calvin minimal runtime on an Arduino Due borad.

##	Installation

In order to use this library the library folder must be placed inside your Arduino libraries.

1. Navigate to `.../arduino-1.x/libraries/` and add the Calvin Arduino library folder.
2. Restart the Arduino application and the Calvin Arduino will be available to include to your sketch.

## Usage

### Example Sketch

####Connection

Since the Calvin Arduino implementation uses ethernet to communicate via the TCP protocol the Arduino needs to be configured to connect to a router or switch. In order to do this the
`setupServer(mac)` or `setupServer(mac, ip)` is used.

`setupServer(mac)` uses DCHP to request an IP-address from the router, while the

`setupServer(mac, ip)` manually asignes the desired IP-address.

Thus one needs to make sure the MAC-address is uniqe on the network.

`uint8_t mac[] = { 0x00, 0xAA, 0xDD, 0x0E, 0xF5, 0x93 };`

#### Output

The standardOut in the example sketch will be on the serial terminal. In order to connect the Arduino to a serial terminal on your computer you will need to set the baud rate to 115200.

If another baudrate is desired one needs to change the following line in the sketch:

`Serial.begin(115200);` to `Serial.begin(XXXX);`



