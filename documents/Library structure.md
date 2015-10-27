
## <center>Calvin Arduino library structure</center>

This library is separated in different sourcefiles according to the following principle.
The main loop and the core of the Calvin implementation is found in CalvinMini.cpp. 
This is where all communication with Calvin base is handled. 
HandleSockets.cpp contains all functions related to sockets and communication with the Ethernet shield. Fifo.cpp contains the actor fifos. Actors.cpp contains all implemented actors, new actors should be written in this file.
