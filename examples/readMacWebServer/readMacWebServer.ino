/*
  Example: readMacWebServer

  This example shows how to read the MAC address from the ATMAC24 EEPROM.
  The first 3 bytes of the MAC will always be 0xFC, 0xC2, and 0x3D. 

  This exmaple will read the MAC and then use act as a simple web server that displays the 
  readings from analog input pins. 
  
  This example requires an ethernet shield. 

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi
  modified by FACTS Engineering

*/

#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>

uint8_t mac[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };   //array to store mac address
IPAddress ip(192, 168, 6, 182);

EthernetServer server(80);

void setup() {
  Ethernet.init(5); //initilize the ethernet port
  
  Serial.begin(9600);
  while(!Serial){
    ;   //wait for serial monitor
  }

  EEPROM.readMacAddress(mac);
  for(int i = 0; i < 6; i++){   //prints mac address in hexidecimal
      Serial.print(mac[i], HEX);
      if(i < 5){
        Serial.print(":");
      }
  }
  Serial.println();

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

    // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
