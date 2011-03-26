/*
  Gyro.cpp - Library for interfacing with the ITG-3200 gyro (specifically, via the Sparkfun IMU Digital Combo Board: SEN-10121)
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
#include "Gyro.h"
#include "I2C.h"

Gyro::Gyro(){
}

void Gyro::init(){
  Serial.println("Initing Gyro");
  _i2c = I2C(GYRO_ADDR);
  
  if (!_i2c.getAddressFromDevice()){
    Serial.println("GYRO NOT CONNECTED!");
  }
  else{
    _i2c.writeSetting(0x3E, 0x80); // Reset it
    _i2c.writeSetting(0x16, 0x1D); // 10Hz low pass filter
    _i2c.writeSetting(0x3E, 0x01); // use X gyro oscillator
  }
}

int Gyro::getTemp(){
  _i2c.sendReadRequest(0x1B);
  temp = _i2c.readWord();
  temp = 35.0 + ((temp + 13200) / 280.0); // -13200 == 35C, 280 == Each degree
  temp = 32 + (temp * 1.8); // Convert to F
  
  return temp;
}

int Gyro::getX(){
  _i2c.sendReadRequest(0x1D);
  x = _i2c.readWord();
  
  return x;
}

int Gyro::getY(){
  _i2c.sendReadRequest(0x1F);
  y = _i2c.readWord();
  
  return y;
}

int Gyro::getZ(){
  _i2c.sendReadRequest(0x21);
  z = _i2c.readWord();
  
  return z;
}
