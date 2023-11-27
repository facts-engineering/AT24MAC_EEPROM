/*
  Example: readSerialNumber

  This example shows how to read the serial number from the ATMAC24 EEPROM.

  The serial number of each ATMAC24 is a unique 128-bit value. 
 
  Written by FACTS Engineering
  Copyright (c) 2023 FACTS Engineering, LLC
  Licensed under the MIT license.
*/

#include <EEPROM.h>

uint8_t serialNum[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  while(!Serial){
    ;   //wait for serial monitor
  }

  EEPROM.readSerialNumber(serialNum);
   for(int i = 0; i< 16; i++){   //prints serial number in hexidecimal
      Serial.print(serialNum[i], HEX);
  }
}

void loop() {

}
