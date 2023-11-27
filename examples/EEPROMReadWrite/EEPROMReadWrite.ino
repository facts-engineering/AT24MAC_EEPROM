/*
  Example: EEPROMReadWrite

  This example shows how to read and write to an individual address of the ATMAC24 EEPROM.

  Written by FACTS Engineering
  Copyright (c) 2023 FACTS Engineering, LLC
  Licensed under the MIT license.
*/

#include <EEPROM.h>

void setup() {
  while(!Serial){
    ;   //wait for serial monitor
  }
}

uint8_t dataAddr = 0x24;
uint8_t outData = 0xA5;
uint8_t readData = 0;

void loop() {

  EEPROM.write(dataAddr, outData);
  readData = EEPROM.read(dataAddr);

  Serial.print("outData is ");
  Serial.println(outData, HEX);   //print outData in hexadecimal
  Serial.print("readData is ");
  Serial.println(readData, HEX);  //print readData in hexadecimal

  while(1);   //infinite loop
}
