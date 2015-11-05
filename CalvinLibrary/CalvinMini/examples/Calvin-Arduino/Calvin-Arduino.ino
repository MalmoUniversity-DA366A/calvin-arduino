/* Calvin-Arduino example sketch

  This example sketch shows how to setup a Calvin runtime on an Arduino Due board.   
  The runtime allows the Arduino to be added to an existing Calvin network.
  Afterwards an compatible actor can be deplayed onto the Arduino from a Calvin base. 
  If aditional actors are descired one can easily modify the library to support this.
  For further information regarding Calvin base, actor development and Calvin-Arduino please visit:
  https://github.com/MalmoUniversity-DA366A/calvin-arduino

*/

#include <ArduinoJson.h>
#include <CalvinMini.h>
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <Adafruit_PN532.h>
#include <Wire.h>


void setup() {
  byte macAdr[] = { 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xFF };  // MAC address for the Ethernet shield
  IPAddress ip(192,168,0,20);                              // Desired IP address for manual IP configuration
  String rtID = "Calvin-Arduino";                          // Name of Calvin runtime
  uint16_t port = 5002;                                    // Server incomming port
  
  Serial.begin(115200);                                    // Setup serial communication
  Serial.println("On"); 
  CalvinMini mini(rtID, macAdr, ip, port);                 // Setup Calvin mini
  mini.loop();                                             // Run the calvin mini loop indefinitely
}

void loop() {
  
}