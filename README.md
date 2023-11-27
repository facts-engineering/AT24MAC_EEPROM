# AT24MAC EEPROM Arduino Library

Arduino library for the AT24MAC402/602 chip. These microchips have build in MAC address and serial number.
Memory size for both parts is 256 bytes.

Use the Arduino Library Manager to install.

## Example
Here is a simple example which shows the capabilities of the library
```cpp
#include <EEPROM.h>

uint8_t mac[6];

void setup() {
	Serial.begin(9600);
  while(!Serial);
  //Read and print MAC address
  EEPROM.readMacAddress(mac);
  Serial.print("MAC address: ");
  for(uint8_t i = 0; i < 6; i++){
    Serial.print(mac[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  //Write 8 bytes and read them back
  for (int i = 0 ; i < 8 ; i++) {
	EEPROM.write(i, 100 + i);
  }
  for (int i = 0 ; i < 8 ; i++) {
	Serial.print("Byte ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(EEPROM.read(i));
	}
}

void loop() {
}
```
