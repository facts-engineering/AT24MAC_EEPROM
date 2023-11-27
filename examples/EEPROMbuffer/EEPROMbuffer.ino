/*
  Example: EEPROMbuffer

  This example shows how to read and write a byte array to the ATMAC24 EEPROM.

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
uint8_t dataBuf[10] = { };
uint8_t readBuf[10] = { };
uint8_t leng = 10;  

void loop() {

  for(uint8_t count = 0; count < leng; count++){
    dataBuf[count] = count;   //fill buffer with data
  }

  EEPROM.write(dataAddr, dataBuf, leng);  //write data buffer to EEPROM
  EEPROM.read(dataAddr, readBuf, leng);   //read data buffer from EEPROM

  Serial.print("dataBuf is ");
  for(int i = 0; i < 10; i++){
      Serial.println(dataBuf[i], HEX);   //print outData in hexadecimal
  }
  Serial.print("readData is ");
  for(int i = 0; i < 10; i++){
      Serial.println(readBuf[i], HEX);  //print readData in hexadecimal
  }

  while(1);   //infinite loop
}
