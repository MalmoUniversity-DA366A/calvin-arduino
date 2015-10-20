#include <ArduinoJson.h>
#include <CalvinMini.h>
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
CalvinMini mini;

// put your setup code here, to run once:
void setup() {
  IPAddress ip(192,168,0,10);
  uint8_t mac[] = { 0x00, 0xAA, 0xDD, 0x0E, 0xF5, 0x93 };
  Serial.begin(115200);
  Serial.println("On");
  mini.setupServer(mac);            //DHCP request
  //mini.setupServer(mac, ip);      //Manual IP-Address configuration
  

}

void loop() {

 mini.loop();      // Run the calvin mini loop indefinitely

}
