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
*/

#include "WProgram.h"
#include "Definitions.h"
#include "Baro.h"
#include "I2C.h"
#include "Utils.h"
#include "EEPROM_lib.h"

Baro::Baro() : I2C(){
}

void Baro::init(){
  //Serial.println("Initing Baro");
  setAddress(BARO_ADDR);
  
  if (!getAddressFromDevice()){
    //Serial.println("BARO NOT CONNECTED!");
  }
  else{
  }
}

// Calculate altitude from the barometer
void Baro::measure(){
}

///////////

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