/*
MIT License

Copyright (c) 2023 FACTS Engineering, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "AT24MAC_EEPROM.h"


EEPROMClass::EEPROMClass(uint8_t _addressPins, TwoWire &INTERNAL_EEPROM_PORT){
	address_lines = _addressPins;
	eeprom_i2c_address = 0x50 | address_lines;
	mac_i2c_address = 0x58 | address_lines;
	eeprom_port = &INTERNAL_EEPROM_PORT;
}
EEPROMClass EEPROM(0b100);

/*******************************************************************************
Description: Reads the MAC address from the AT24MAC EEPROM. THe first 3 bytes of
			 the  MAC address will always be  0xFC, 0xC2, 0x3D.

Parameters: -uint8_t buffer to store the mac address

Returns: 	-none
*******************************************************************************/

void EEPROMClass::readMacAddress(uint8_t mac[]) {
  if(initialized == false){
	EEPROM.initialize();
  }
	
  eeprom_port->beginTransmission(mac_i2c_address);
  eeprom_port->write(EUI48_LOCATION); // Location of the MAC Address
  eeprom_port->endTransmission();

  eeprom_port->requestFrom(mac_i2c_address, MAC_ADDR_LENGTH);
  for(int i = 0; i < MAC_ADDR_LENGTH; i++) {
    mac[i] = eeprom_port->read();
  }
  
  return;
}

/*******************************************************************************
Description: Reads the serial number from the AT24MAC EEPROM

Parameters: -uint8_t buffer to store the serial number

Returns: 	-none
*******************************************************************************/

void EEPROMClass::readSerialNumber(uint8_t serialNumber[]) {
  if(initialized == false){
	EEPROM.initialize();
  }
  
  eeprom_port->beginTransmission(mac_i2c_address);	//Uses same device address as MAC
  eeprom_port->write(SERIAL_NUMBER_LOCATION); // Location of the MAC Address
  eeprom_port->endTransmission();

  eeprom_port->requestFrom(mac_i2c_address, SERIAL_NUMBER_LENGTH);
  for(int i = 0; i < SERIAL_NUMBER_LENGTH; i++) {
    serialNumber[i] = eeprom_port->read();
  }
  
  return;
}

/*******************************************************************************
Description: Reads the data from the address given. 

Parameters: -uint8_t dataAddress: the location to be read

Returns: 	-none
*******************************************************************************/

uint8_t EEPROMClass::read(uint8_t dataAddress){
  if(initialized == false){
	EEPROM.initialize();
  }	

  eeprom_port->beginTransmission(eeprom_i2c_address);
  eeprom_port->write(dataAddress); 
  eeprom_port->endTransmission();
  
  eeprom_port->requestFrom(eeprom_i2c_address, 1);
  return eeprom_port->read();
}

/*******************************************************************************
Description: Reads the data into a buffer starting from the address given. 

Parameters: -Starting data address, data buffer to store data, and length 
			 of data

Returns: 	-none
*******************************************************************************/

void EEPROMClass::read(uint8_t dataAddress, uint8_t dataBuf[], uint8_t length){
  if(initialized == false){
	EEPROM.initialize();
  }
  
  eeprom_port->beginTransmission(eeprom_i2c_address);
  eeprom_port->write(dataAddress); 
  eeprom_port->endTransmission();
  
  eeprom_port->requestFrom(eeprom_i2c_address, length);
  for(int i = 0; i < length; i++){
	 dataBuf[i] = eeprom_port->read();
  }
	
  return; 
}

/*******************************************************************************
Description: Writes the provided data to the address given 

Parameters: -address to write data and data to be written

Returns: 	-none
*******************************************************************************/

void EEPROMClass::write(uint8_t dataAddress, uint8_t data){
  if(initialized == false){
	EEPROM.initialize();
  }

  eeprom_port->beginTransmission(eeprom_i2c_address);
  eeprom_port->write(dataAddress); 
  eeprom_port->write(data); 
  eeprom_port->endTransmission();
  delay(WRITE_TIME);	//write settling time (t_wr)
  
  return; 
}

/*******************************************************************************
Description: Writes the provided data buffer starting at the address given and 
			 ending at after length is reached.

Parameters: -starting address, data buffer to be written, length of the data buffer

Returns: 	-none
*******************************************************************************/

void EEPROMClass::write(uint8_t dataAddress, uint8_t dataBuf[], uint8_t length){
  uint8_t startByte = dataAddress % PAGE_SIZE;
  int currentByte = startByte;
  
  if(initialized == false){
	EEPROM.initialize();
  }
	
	eeprom_port->beginTransmission(eeprom_i2c_address);
	eeprom_port->write(dataAddress);
	for(int j = 0; j < length; j++)
	{
		if((currentByte % (PAGE_SIZE)) != 0){		//increment until end of page
			eeprom_port->write(dataBuf[j]);
		}
		else{
			eeprom_port->endTransmission();
			delay(WRITE_TIME);	//write settling time (t_wr)
			
			eeprom_port->beginTransmission(eeprom_i2c_address);
			eeprom_port->write(dataAddress + j);
			eeprom_port->write(dataBuf[j]);
		}
		currentByte++;
	}
	eeprom_port->endTransmission();
	delay(WRITE_TIME);	//write settling time (t_wr)
 
  return; 
}

/*******************************************************************************
Description: Checks existing data and in the event that new data does not match
			 existing data will write new data

Parameters: -address to write data and data to be written

Returns: 	-none
*******************************************************************************/

void EEPROMClass::update(uint8_t dataAddress, uint8_t data){
  if(initialized == false){
    EEPROM.initialize();
  } 
	
  uint8_t currentData = read(dataAddress);  
  if(currentData != data){
    write(dataAddress, data);
  }
  else{
    ;
  }

  return; 
}

void EEPROMClass::initialize(){
	 INTERNAL_EEPROM_PORT.begin();
	 initialized = true;
}

/*******************************************************************************
Description: Returns the number of cells in the EEPROM

Parameters: -uint8_t dataAddress: the location to be read

Returns: 	-none
*******************************************************************************/

uint8_t EEPROMClass::length(){
  return EEPROM_LENGTH;
}



