
# <center>Calvin Arduino library structure</center>

####This library is separated in different sourcefiles according to the following principle:

- The following .cpp files with corresponding .h  files are located within the `...\CalvinMini\src` folder.

 - **CalvinMini.cpp**
Handles the main loop and makes the core of the Calvin implementation. This is where all messages to and from Calvin base is decoded and where all communication is handled.

 - **HandleSockets.cpp**
contains all functions related to sockets and TCP/IP communication with the Ethernet shield. 

 - **Fifo.cpp**
contains the actor fifos and handles incomming and outgoing tokens from and to Calvin base. 

 - **Actors.cpp**
contains all implemented actors, new actors and their functions should be written in this file.

- In the `...\CalvinMini\examples` folder an example sketch is provided to show an easy implementation of the Calvin mini runtime on the Arduino.

- In the `...\CalvinMini` folder the **library.properites**, **kewords.txt** and the **README.adoc** files are located. These files describes the library and are used by the Arduino IDE.  