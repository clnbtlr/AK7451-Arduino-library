# AK7451-Arduino-library
Arduino library for reading rotation angle from AK7451 sensor. This IC by Asahi Kasei Microdevices Corporation (AKM) uses a built-in Hall effect element to perform non-contact rotation angle measurements in combination with a diametrically magnetized two pole magnet.
Communication is via SPI.

The sensor was tested using the Mikroe-2755 Angle 3 click breakout board with an Arduino Mirco and Arduino Uno as the sensor uses 5V. A diametrically magnetised ring magnet Ø 10/5 mm from supermagnete was used.

The pinout for the connections is as follows: 

| Arduino micro            | AK7451/Angle 3 click |
| ------------------------ | -------------------- |
| 5V   | 5V  | 
| GND  | GND | 
| SCK  | SCK |
| MI   | SDO |
| MO   | SDI | 
| SS   | CS  |

Currently only basic usage is implemented. This includes reading the absolute angle data, error and mode bits in Normal Mode (see the AK7451 datasheet for more details). User Mode is a work in progress.

Library installation in Arduino IDE is per the usual manner.
