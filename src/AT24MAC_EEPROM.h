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

#ifndef AT24MAC_EEPROM_h
#define AT24MAC_EEPROM_h
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

#define EEPROM_LENGTH 0xFF
#define PAGE_SIZE 16
#define WRITE_TIME 5

#define EUI48_LOCATION 0x9A
#define MAC_ADDR_LENGTH 6
#define SERIAL_NUMBER_LOCATION 0x80
#define SERIAL_NUMBER_LENGTH 16

#ifndef INTERNAL_EEPROM_PORT
	#define INTERNAL_EEPROM_PORT Wire	//Assume using standard I2C port and external device
#endif

class EEPROMClass
{
public:
	EEPROMClass(uint8_t _addressPins = 0, TwoWire &wirePort = INTERNAL_EEPROM_PORT);

	uint8_t address_lines;
	uint8_t eeprom_i2c_address;
	uint8_t mac_i2c_address;
	TwoWire *eeprom_port;


	void readMacAddress(uint8_t mac[]);
	void readSerialNumber(uint8_t serialNumber[]);
	
	uint8_t read(uint8_t dataAddress);
	void read(uint8_t dataAddress, uint8_t dataBuf[], uint8_t length);
	void write(uint8_t dataAddress, uint8_t data);
	void write(uint8_t dataAddress, uint8_t dataBuf[], uint8_t length);
	void update(uint8_t dataAddress, uint8_t data);
	uint8_t length();


	template <typename T>
	void get(uint8_t dataAddress, T &data){
		uint8_t *ptr = (uint8_t*) &data; 
		uint16_t dataSize = sizeof(T);
		if(dataSize + dataAddress > EEPROM_LENGTH + 1){
			return;
		}
		for(int i = 0; i < dataSize; i++){
			*ptr = read(dataAddress + i);
			ptr++;	
		}
	}
	
	template <typename T>
	void put(uint8_t dataAddress, T &data){
		uint8_t *ptr = (uint8_t*) &data; 
		uint16_t dataSize = sizeof(T);
		if(dataSize + dataAddress > EEPROM_LENGTH + 1){
			return;
		}
		for(int i = 0; i < dataSize; i++){
			 update(dataAddress + i, *ptr);
			 ptr++;	
		}
	}
private:
	bool initialized = false;
	void initialize();
	
};

extern EEPROMClass EEPROM;
#endif
