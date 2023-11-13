/***************************************************************************
* File Name: ak7451.cpp
* Processor/Platform: Arduino Uno R3 (tested)
* Development Environment: Arduino 2.2.1
*
* Designed for use with with AK7451 magnetic angle sensor IC
* breakout boards: Mikroe Angle 3 Click
*
* Copyright © 2023
* SOFTWARE LICENSE AGREEMENT: This code is released under the MIT License.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
* **************************************************************************/

#include "AK7451.h"

AK7451::AK7451() :
  _spiSettings(2000000, MSBFIRST, SPI_MODE2)
{
}

void AK7451::begin(int8_t chipSelectPin, SPIClass &spiPort)
{
  // Function to initialize channel, load private variables
  _cs = chipSelectPin;
  _spiPort = &spiPort;

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH); // immediately pull CS pin high to avoid conflicts on SPI bus
  _spiPort->begin();
}

void AK7451::writeData(uint8_t opcode, uint8_t reg, uint16_t data)
// seems to work but dnt understand return from reg
{
  //uint8_t txBuf[3] = {0,0,0};

  reg <<= 1; // register address is 7 bits

  uint8_t txBuf0 = ( opcode << 4 ) | ( reg >> 4 );
  uint8_t txBuf1 = ( reg << 4 ) | (( data >> 8 ) & 0x0F );
  uint8_t txBuf2 = data & 0xFF;

  _spiPort->beginTransaction(_spiSettings);
  digitalWrite(_cs, LOW);           // set pin low to start talking to IC
  
  _spiPort->transfer(txBuf0);       // transfer command
  _spiPort->transfer(txBuf1);
  _spiPort->transfer(txBuf2);

  digitalWrite(_cs, HIGH);          // set pin high to end SPI session
  _spiPort->endTransaction();
}

uint16_t AK7451::readData(uint8_t opcode, uint8_t reg)
{
  uint8_t txBuf = 0;
  uint8_t rxBuf[2] = {0,0};
  uint16_t rawData = 0;

  reg <<= 1; // register address is 7 bits
  txBuf = ( opcode << 4 ) | ( reg >> 4 );

  _spiPort->beginTransaction(_spiSettings);  
  digitalWrite(_cs, LOW);           // set pin low to start talking to IC

  _spiPort->transfer(txBuf);       // transfer command
  rxBuf[0] = _spiPort->transfer(0);   // read first byte of data from IC
  rxBuf[1] = _spiPort->transfer(0);   // read second byte of data from IC

  digitalWrite(_cs, HIGH);          // set pin high to end SPI session
  _spiPort->endTransaction();

  rawData = rxBuf[0];
  rawData <<=8;
  rawData |= rxBuf[1];

  return rawData;
}

float AK7451::readAngle()
{
  rawData = readData(OPCODE_READ_ANGLE, REG_ANG);
  rawData &= 0x0FFF; // angle data is 12 bits
  float angle = (360*(float)rawData)/4096;

  return angle;
}

bool AK7451::readErrorBit()
{
  // E bit is 1 if Normal
  // E bit is 0 if Abnormal
  rawData = readData(OPCODE_READ_ANGLE, REG_ANG);
  rawData >>= 12; // remove last 12 bits
  rawData &= 0x01; // remove first 3 bits

  return rawData;
}

bool AK7451::readModeBit()
{
  // MD bit is 0 in Normal Mode
  // MD bit is 1 in User Mode
  rawData = readData(OPCODE_READ_ANGLE, REG_ANG);
  rawData >>= 15; // remove last 15 bits

  return rawData;
}

float AK7451::readMagFlux() // not working
{
  rawData = readData(OPCODE_READ_REG, REG_MAG);
  rawData &= 0x3F; // mag data is 7 bits // issue is here
  float magFlux = (float)rawData; // value in mT in range 0-127. For reference only.

  return magFlux;
}