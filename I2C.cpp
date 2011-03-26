/*
  I2C.cpp - Convenience library for dealing with I2C-connected sensors
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
#include "I2C.h"
#include <Wire.h>

I2C::I2C(){
  Wire.begin();
}

I2C::I2C(int address){
  Wire.begin();
  _address = address;
}

int I2C::getAddressFromDevice(){
  sendReadRequest(0x00);
  return readByte();
}

boolean I2C::validateDevice(){
  int got = getAddressFromDevice();
  return got == _address ? true : false;
}

int I2C::writeSetting(byte data_address, byte data_value){
  Wire.beginTransmission(_address);
  Wire.send(data_address);
  Wire.send(data_value);
  return Wire.endTransmission();
}

int I2C::sendReadRequest(int data_address){
  Wire.beginTransmission(_address);
  Wire.send(data_address);
  return Wire.endTransmission();
}

word I2C::readWord(){
  Wire.requestFrom(_address, 2);
  return ((Wire.receive() << 8) | Wire.receive());
}

byte I2C::readByte(){
  Wire.requestFrom(_address, 1);
  return Wire.receive();
}

