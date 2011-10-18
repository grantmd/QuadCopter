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
*/

#include "WProgram.h"
#include "Definitions.h"
#include "Mag.h"
#include "I2C.h"
#include "Utils.h"
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
    writeSetting(0x02, 0x00); // continuous measurement mode
  }
}

// Updates all raw measurements from the magnetometer
void Mag::updateAll(){
  sendReadRequest(0x03);
  requestBytes(6);

  for (byte axis = XAXIS; axis <= ZAXIS; axis++){
     dataRaw[axis] = readNextWordFlip();
  }
}

///////////

int Mag::getRaw(byte axis){
  return dataRaw[axis];
}
