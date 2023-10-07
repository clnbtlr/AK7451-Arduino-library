/* AK7451 angle sensor basic usage example
CButler. Oct 2023.

Using Arduino Micro as MCB as sensor requires 5V.
Pinout:
  AK7451:
  5V <-> 5V // probably need to change this to do reset or set normal mode at start
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
  Serial.println("***AK7451 Sensor Basic Usage Test***");
  ak7451.begin(SS); // SS pin on Arduino Micro but can be any other Digital I/O Pin
}

void loop() {
  float angle = ak7451.readAngle(); // read angle from sensor
  Serial.println(angle); // print angle to Serial Monitor
  delay(1000); // delay 1 second between measurements 
}
