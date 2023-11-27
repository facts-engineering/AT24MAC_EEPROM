/*
  Example: EEPROMbuffer

  This example shows how to use the update function to change the value stored 
  in an address to the ATMAC24 EEPROM.
  In the event that the existing data is not different to the new data a write 
  operation will not occur. This is to limit wear on the EEPROM. 

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
uint8_t outData = 0;
uint8_t readData = 0; 

void loop() {
  EEPROM.write(dataAddr, outData);  //write data to EEPROM
  readData = EEPROM.read(dataAddr);   //read data from EEPROM
  Serial.print("outData is ");
  Serial.print(outData, HEX);   //print outData in hexadecimal
  Serial.println(" before update. ");
  Serial.print("readData is ");
  Serial.print(readData, HEX);  //print readData in hexadecimal
  Serial.println(" before update. ");
  
  outData = 0xA5;
  EEPROM.update(dataAddr, outData);   //update the value at dataAddr
  readData = EEPROM.read(dataAddr);   //read data from EEPROM
  Serial.print("outData is ");
  Serial.print(outData, HEX);   //print outData in hexadecimal
  Serial.println(" after update. ");
  Serial.print("readData is ");
  Serial.print(readData, HEX);  //print readData in hexadecimal
  Serial.println(" after update. ");

  while(1);   //infinite loop
}
