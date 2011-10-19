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
}

void I2C::setAddress(byte address){
  _address = address;
}

// The two sensors I have both let you read their address from register 0
byte I2C::getAddressFromDevice(){
  sendReadRequest(0x00);
  return readByte();
}

// Verify that the address we get back from the device matches what we have
boolean I2C::validateDevice(){
  byte got = getAddressFromDevice();
  return got == _address ? true : false;
}

// Write a setting to the device at register data_address
byte I2C::writeSetting(byte data_address, byte data_value){
  Wire.beginTransmission(_address);
  Wire.send(data_address);
  Wire.send(data_value);
  return Wire.endTransmission();
}

// Tell the device that we will be reading from register data_address
byte I2C::sendReadRequest(byte data_address){
  Wire.beginTransmission(_address);
  Wire.send(data_address);
  return Wire.endTransmission();
}

// Request 2 bytes and read it
word I2C::readWord(){
  requestBytes(2);
  return ((Wire.receive() << 8) | Wire.receive());
}

// Request 2 bytes and read it
word I2C::readWordFlip(){
  requestBytes(2);
  byte msb = Wire.receive();
  byte lsb = Wire.receive();
  return ((lsb << 8) | msb);
}

// Request a byte and read it
byte I2C::readByte(){
  requestBytes(1);
  return Wire.receive();
}

// Request some number of bytes
void I2C::requestBytes(byte bytes){
  Wire.beginTransmission(_address);
  Wire.requestFrom(_address, bytes);
}

// Read the next available byte
byte I2C::readNextByte(){
  return Wire.receive();
}

// Read the next available 2 bytes
word I2C::readNextWord(){
  return ((Wire.receive() << 8) | Wire.receive());
}

// Read the next available 2 bytes
word I2C::readNextWordFlip(){
  byte msb = Wire.receive();
  byte lsb = Wire.receive();
  return ((lsb << 8) | msb);
}

