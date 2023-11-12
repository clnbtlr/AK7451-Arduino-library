/* AK7451 angle sensor user mode usage example
CButler. Oct 2023.

Using Arduino Micro as MCB as sensor requires 5V.
Pinout:
  AK7451:
  5V <-> 5V
  GND <-> GND
  SCK <-> SCK
  MI <-> SDO
  MO <-> SDI
  SS <-> CS
*/
#include "ak7451.h"

AK7451  ak7451; // initialise the sensor
void setup() {
  Serial.begin(115200);
  while(!Serial){ // wait for Serial Monitor
    delay(1);
  }
  Serial.println("***AK7451 Sensor User Mode Usage Test***");
  ak7451.begin(SS); // SS pin on Arduino Micro but can be any other Digital I/O Pin
  
  ak7451.writeData(0x05,0x02,0x0000); // write data to sensor (change to Normal  Mode)
  bool mode = ak7451.readModeBit();
  Serial.print(","); Serial.println(mode);

  delay(2000);
  ak7451.writeData(0x05,0x02,0x050F); // write data to sensor (change to User Mode)
  delay(100);
}

void loop() {
  float magFlux = ak7451.readMagFlux();
  Serial.print(magFlux); // print angle to Serial Monitor
  bool mode = ak7451.readModeBit();
  Serial.print(","); Serial.print(mode);
  uint16_t rawData = ak7451.readData(0x04,0x02);
  //rawData &= 0x0FFF;
  //rawData = rawData & 0xFFF;
  Serial.print(","); Serial.println(rawData,BIN);
  delay(1000); // delay 1 second between measurements 
}
