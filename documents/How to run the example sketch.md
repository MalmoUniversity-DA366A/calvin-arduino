# Calvin Mini Arduino Library


## Description

The Calvin Arduino library is an implementation of Ericsson's Calvin minimal. Calvin Arduino enables you to run a Calvin minimal runtime on an Arduino Due borad.


##	Installation

In order to use this library the library folder must be placed inside your Arduino libraries.

1. Navigate to `.../arduino-1.x/libraries/` and add the Calvin Arduino library folder. _(If the folder does not exist, simply add the folder)._
2. Restart the Arduino application and the Calvin Arduino will be available to include to your sketch.

## Usage

### Example Sketch

####Connection

Since the Calvin Arduino implementation uses ethernet to communicate via the TCP protocol the Arduino needs to be configured to connect to a router or switch. In order to do this the proper  MAC address and IP address must be filled in. The MAC address is located on a sticker on your Ethernet shiled, if not simply choose a uniqe one. The IP address must be within the IP range of your router. 

- To **change** the **IP address** simply alter this line in the sketch so that it matches an unused IP address within your routers IP range:

	`IPAddress ip(192,168,0,20);` to **`IPAddress ip(192,168,xx,xx);`**

- To **change** the **MAC address** simply alter this line in the sketch:

	`byte macAdr[] = { 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xFF };`

- Apart from configuring the Ethernet shield one also need to **select** which **port** the Arduino should listen on to serve incomming connection. This is done by changing the following line:

	`uint16_t port = 5002;` to **`uint16_t port = xxxx;`**

	**(Note that:** the Calvin base **must** be told to connect to this port as well.**)**

- The final part of the Calvin mini configuration is to name the Calvin runtime. This is done by altering the string on the following line:

	`String rtID = "Calvin-Arduino";` to **`String rtID = "My_runtime_name";`**

Once these steps are done the Calvin Mini is initiated with the following line:

`CalvinMini mini(rtID, macAdr, ip, port);`

  

#### Output

The standardOut in the example sketch will be on the serial terminal. In order to connect the Arduino to a serial terminal on your computer you will need to set the baud rate to 115200.

- If another baudrate is desired one needs to change the following line in the sketch:

	`Serial.begin(115200);` to **`Serial.begin(XXXX);`**

#### Starting the runtime
When the initiation is completed the Calvin Arduino runtime is started by the following line: `mini.loop();`. Once this function call has been made the entire program is stuck within the Calvin runtime loop and no further setup can be done. Hence this line should be place **after** all initial setup. 


