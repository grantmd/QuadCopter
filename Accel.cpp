/*
  Accel.cpp - Library for interfacing with the ADXL345 accelerometer (specifically, via the Sparkfun IMU Digital Combo Board: SEN-10121)
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
#include "Accel.h"
#include "I2C.h"
#include "Utils.h"

Accel::Accel(){
  _scaleFactor = G_2_MPS2(4.0 / 1024); // ±2 g is a range of 4, converted to Gs, and then converted to m/s2
  _smoothFactor = 1.0;
  _i2c = I2C();
}

void Accel::init(){
  Serial.println("Initing Accel");
  _i2c.setAddress(ACCEL_ADDR);
  
  if (!_i2c.getAddressFromDevice()){
    Serial.println("ACCEL NOT CONNECTED!");
  }
  else{
    _i2c.writeSetting(0x2D, 0x08); // Power up, measure mode
    _i2c.writeSetting(0x2C, 0x0A); // 100Hz low pass filter
    _i2c.writeSetting(0x31, 0x00); // ±2 g
    
    // TODO: We should calculate these once under known conditions and store them in eeprom
    //autoZero();
  }
}

// Calculate zero for all 3 axis, storing it for later measurements
// This assumes your platform is truly level!
void Accel::autoZero(){
  // Take 50 measurements of all 3 axis, find the median, that's our zero-point
  // Why 50? Because that's what the aeroquad project does
  // (note, I could not get to 50 -- my arduino locks up. so I'm leaving it at 10)
  byte loopCount = 10;
  
  Serial.print("Starting accel autoZero with ");
  Serial.print(loopCount, DEC);
  Serial.println(" iterations.");
  int findZero[loopCount];
  for (byte axis = PITCH; axis <= YAW; axis++){
    for (byte i=0; i<loopCount; i++){
      _i2c.sendReadRequest(0x32 + (axis * 2));
      findZero[i] = _i2c.readWord();
      delay(10);
    }
    
    zero[axis] = findMedian(findZero, loopCount);
    Serial.print("Zero of accel axis ");
    Serial.print(axis, DEC);
    Serial.print(" is: ");
    Serial.println(zero[axis]);
  }
}

// Updates all raw measurements from the accelerometer
void Accel::updateAll(){
  //Serial.println("Updating all accel data");
  _i2c.sendReadRequest(0x32);
  _i2c.requestBytes(6);

  for (byte axis = PITCH; axis <= YAW; axis++) {
     dataRaw[axis] = zero[axis] - _i2c.readNextWord();
     dataSmoothed[axis] = filterSmooth((float)dataRaw[axis] * _scaleFactor, dataSmoothed[axis], _smoothFactor);
  }
  
  _lastMeasureTime = micros();
}

///////////

int Accel::getRawPitch(){
  return dataRaw[PITCH];
}

int Accel::getRawRoll(){
  return dataRaw[ROLL];
}

int Accel::getRawYaw(){
  return dataRaw[YAW];
}

///////////

int Accel::getPitch(){
  return dataSmoothed[PITCH];
}

int Accel::getRoll(){
  return dataSmoothed[ROLL];
}

int Accel::getYaw(){
  return dataSmoothed[YAW];
}

