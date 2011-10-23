/*
  Mag.cpp - Library for interfacing with the HMC5883L Triple Axis Magnetometer from Sparkfun:
  http://www.sparkfun.com/products/10530
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

    writeSetting(0x02, 0x00); // continuous measurement mode
  }
}

// Updates all raw measurements from the magnetometer
void Mag::updateAll(){
  // TODO: pass in pitch and roll to apply tilt compensation at measurement time
  // TODO: take calibration into account

  sendReadRequest(0x03);
  requestBytes(6);

  // annoyingly, the registers are actually x,z,y (from the datasheet)
  dataRaw[XAXIS] = readNextWordFlip();
  dataRaw[ZAXIS] = readNextWordFlip();
  dataRaw[YAXIS] = readNextWordFlip();
}

///////////

int Mag::getRaw(byte axis){
  return dataRaw[axis];
}

// Our compass heading, in radians
float Mag::getHeading(){
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(getRaw(YAXIS), getRaw(XAXIS));
   
  // Correct for when signs are reversed.
  if (heading < 0)
    heading += (2 * PI);
  
  // TODO: Correct for declination at my house
  return heading;
}

// Our compass heading in degrees
float Mag::getHeadingDegrees(){
  return getHeading() * 180 / PI;
}
