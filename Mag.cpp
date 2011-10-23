/*
  Mag.cpp - Library for interfacing with the HMC5883L Triple Axis Magnetometer from Sparkfun:
  http://www.sparkfun.com/products/10530
  http://www.sparkfun.com/products/9441 - HMC5843 works as well
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
  https://www.loveelectronics.co.uk/Tutorials/8/hmc5883l-tutorial-and-arduino-library
  https://www.loveelectronics.co.uk/Tutorials/13/tilt-compensated-compass-arduino-tutorial
*/

#include "WProgram.h"
#include "Definitions.h"
#include "Mag.h"
#include "I2C.h"
#include "EEPROM_lib.h"

Mag::Mag() : I2C(){
}

void Mag::init(){
  //Serial.println("InitingMagBaro");
  setAddress(MAG_ADDR);
  
  if (!getAddressFromDevice()){
    //Serial.println("MAG NOT CONNECTED!");
  }
  else{
    // TODO: put in self-test and calibrate

    writeSetting(0x01, 0x01<<5); // 1.0 gauss scale
    _scale = 1.0;
    writeSetting(0x02, 0x00); // continuous measurement mode
  }
}

// Updates all raw measurements from the magnetometer
void Mag::updateAll(float roll, float pitch){
  // TODO: take calibration into account

  sendReadRequest(0x03);
  requestBytes(6);

  // annoyingly, the registers are actually x,z,y (from the datasheet)
  dataRaw[XAXIS] = readNextWordFlip() * _scale;
  dataRaw[ZAXIS] = readNextWordFlip() * _scale;
  dataRaw[YAXIS] = readNextWordFlip() * _scale;
  
  // apply tilt compensation
  // TODO: check signs on roll/pitch vs mag to make sure we're all speaking the same language
  roll = roll * PI / 180; // to radians
  pitch = pitch * PI / 180; // to radians
  
  // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);  
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);
  
  float Xh = dataRaw[XAXIS] * cosPitch + dataRaw[ZAXIS] * sinPitch;
  float Yh = dataRaw[XAXIS] * sinRoll * sinPitch + dataRaw[YAXIS] * cosRoll - dataRaw[ZAXIS] * sinRoll * cosPitch;
  
  _heading = atan2(Yh, Xh);
   
  // Correct for when signs are reversed.
  if (_heading < 0)
    _heading += (2 * PI);
}

///////////

int Mag::getRaw(byte axis){
  return dataRaw[axis];
}

// Our compass heading, in radians
float Mag::getHeading(){
  // TODO: Correct for declination at my house
  return _heading;
}

// Our compass heading in degrees
float Mag::getHeadingDegrees(){
  return getHeading() * 180 / PI;
}
