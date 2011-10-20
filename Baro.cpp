/*
  Baro.cpp - Library for interfacing with the BMP085 barometer from Sparkfun:
  http://www.sparkfun.com/products/9694
  Created by Myles Grant <myles@mylesgrant.com>
  See also: https://github.com/grantmd/QuadCopter
  
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 

  A good tutorial:
  http://www.sparkfun.com/tutorials/253
*/

#include "WProgram.h"
#include "Definitions.h"
#include "Baro.h"
#include "I2C.h"
#include "Utils.h"
#include "EEPROM_lib.h"

const float _p0 = 101325;     // Pressure at sea level (Pa)

Baro::Baro() : I2C(){
  // oversampling setting
  // 0 = ultra low power
  // 1 = standard
  // 2 = high
  // 3 = ultra high resolution
  _overSamplingSetting = 3;
}

void Baro::init(){
  //Serial.println("Initing Baro");
  setAddress(BARO_ADDR);
  
  if (!getAddressFromDevice()){
    //Serial.println("BARO NOT CONNECTED!");
  }
  else{
    // Read calibration data
    // The barometer is calibrated at the factory, and those settings are written to EEPROM
    sendReadRequest(0xAA);
    _ac1 = readWord();
    sendReadRequest(0xAC);
    _ac2 = readWord();
    sendReadRequest(0xAE);
    _ac3 = readWord();
    sendReadRequest(0xB0);
    _ac4 = readWord();
    sendReadRequest(0xB2);
    _ac5 = readWord();
    sendReadRequest(0xB4);
    _ac6 = readWord();
    sendReadRequest(0xB6);
    _b1 = readWord();
    sendReadRequest(0xB8);
    _b2 = readWord();
    sendReadRequest(0xBA);
    _mb = readWord();
    sendReadRequest(0xBC);
    _mc = readWord();
    sendReadRequest(0xBE);
    _md = readWord();
  }
}

// Calculate altitude from the barometer
void Baro::measure(){
  // read uncompensated temperature
  readUT();

  // read uncompensated pressure
  readUP();

  // calculate true temperature
  long x1, x2, b5;
  
  x1 = (((long)_ut - (long)_ac6) * (long)_ac5) >> 15;
  x2 = ((long)_mc << 11)/(x1 + _md);
  b5 = x1 + x2;

  _temp = ((b5 + 8) >> 4);  

  // calculate true pressure
  long x3, b3, b6;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (_b2 * (b6 * b6) >> 12) >> 11;
  x2 = (_ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((long)_ac1) * 4 + x3) << _overSamplingSetting) + 2) >> 2;
  
  // Calculate B4
  x1 = (_ac3 * b6) >> 13;
  x2 = (_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (_ac4 * (unsigned long)(x3 + 32768)) >> 15;
  
  b7 = ((unsigned long)(_up - b3) * (50000 >> _overSamplingSetting));
  if (b7 < 0x80000000)
    _pressure = (b7 << 1) / b4;
  else
    _pressure = (b7 / b4) << 1;
    
  x1 = (_pressure >> 8) * (_pressure >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * _pressure) >> 16;
  _pressure += (x1 + x2 + 3791) >> 4;

  // convert pressure to altitude in meters
  _altitude = (float)44330 * (1 - pow(((float) _pressure / _p0), 0.190295));
}

void Baro::readUT(){  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  writeSetting(0xF4, 0x2E);
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  sendReadRequest(0xF6);
  _ut = readWord();
}

void Baro::readUP(){
  unsigned char msb, lsb, xlsb;
  
  // Write 0x34+(_overSamplingSetting<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  writeSetting(0xF4, 0x34 + (_overSamplingSetting<<6));
  
  // Wait for conversion, delay time dependent on OSS
  delay(26);

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  sendReadRequest(0xF6);
  requestBytes(3);
  
  msb = readNextByte();
  lsb = readNextByte();
  xlsb = readNextByte();
  
  _up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8 - _overSamplingSetting);
}

///////////

short Baro::getTemp(){
  return _temp;
}

long Baro::getPressure(){
  return _pressure;
}

// How far off the ground are we, in meters?
float Baro::getAltitude(){
  return getRawAltitude() - getGroundAltitude();
}

// Where's the ground, in meters? (Where we took off from)
float Baro::getGroundAltitude(){
  return _groundAltitude;
}

// Raw altitude above sea level, in meters
float Baro::getRawAltitude(){
  return _altitude;
}

///////////

void Baro::setGroundAltitude(){
  _groundAltitude = getRawAltitude();
}