/*
  Baro.cpp - Library for interfacing with the BMP085 barometer
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

// Updates all raw measurements from the barometer
void Baro::updateAll(){
}

///////////

