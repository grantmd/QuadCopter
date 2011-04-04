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
#include "EEPROM.h"

Accel::Accel() : I2C(){
  _scaleFactor = G_2_MPS2(4.0 / 1024); // ±2 g is a range of 4, converted to Gs, and then converted to m/s2
  _smoothFactor = 1.0;
  _oneG = 9.80665;
}

void Accel::init(){
  //Serial.println("Initing Accel");
  setAddress(ACCEL_ADDR);
  
  if (!getAddressFromDevice()){
    Serial.println("ACCEL NOT CONNECTED!");
  }
  else{
    writeSetting(0x2D, 0x00); // Shut down
    writeSetting(0x2D, 0x16); // Reset
    writeSetting(0x2D, 0x08); // Power up, measure mode
    writeSetting(0x2C, 0x0A); // 100Hz low pass filter
    writeSetting(0x31, 0x00); // ±2 g
    
    // Load calibration data from eeprom
    calibrate();
  }
}

void Accel::calibrate(){
  // load from eeprom
  zero[XAXIS] = eeprom_read_int(EEPROM_ADDR_ACCEL_PITCH);
  zero[YAXIS] = eeprom_read_int(EEPROM_ADDR_ACCEL_ROLL);
  zero[ZAXIS] = eeprom_read_int(EEPROM_ADDR_ACCEL_YAW);
  
  /*Serial.print("Accel zeros: ");
  Serial.print(zero[PITCH]);
  Serial.print(",");
  Serial.print(zero[ROLL]);
  Serial.print(",");
  Serial.println(zero[YAW]);*/
  
  // We need to recalc what 1G feels like
  updateAll();
  _oneG = zero[ZAXIS];
}

// Calculate zero for all 3 axis, storing it for later measurements
// This assumes your platform is truly level!
void Accel::autoZero(){
  // Take 50 measurements of all 3 axis, find the median, that's our zero-point
  // Why 50? Because that's what the aeroquad project does
  // (note, I could not get to 50 -- my arduino locks up. so I'm leaving it at 10)
  byte loopCount = 10;
  
  //Serial.print("Starting accel autoZero with ");
  //Serial.print(loopCount, DEC);
  //Serial.println(" iterations.");
  int findZero[loopCount];
  for (byte axis = XAXIS; axis <= ZAXIS; axis++){
    for (byte i=0; i<loopCount; i++){
      sendReadRequest(0x32 + (axis * 2));
      findZero[i] = readWordFlip();
      delay(10);
    }
    
    zero[axis] = findMedian(findZero, loopCount);
    /*Serial.print("Zero of accel axis ");
    Serial.print(axis, DEC);
    Serial.print(" is: ");
    Serial.println(zero[axis]);*/
  }
  
  // Write to eeprom
  eeprom_write(EEPROM_ADDR_ACCEL_PITCH, zero[XAXIS]);
  eeprom_write(EEPROM_ADDR_ACCEL_ROLL, zero[YAXIS]);
  eeprom_write(EEPROM_ADDR_ACCEL_YAW, zero[ZAXIS]);
  
  _oneG = zero[ZAXIS];
}

// Updates all raw measurements from the accelerometer
void Accel::updateAll(){
  //Serial.println("Updating all accel data");
  sendReadRequest(0x32);
  requestBytes(6);

  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
     dataRaw[axis] = zero[axis] - readNextWordFlip();
     dataSmoothed[axis] = filterSmooth((float)dataRaw[axis] * _scaleFactor, dataSmoothed[axis], _smoothFactor);
  }
  
  _lastMeasureTime = micros();
}

///////////

//
// Now, either I am doing something wrong, there is something wrong with my board, or I don't understand how these things work...
// But the pitch/roll/yaw terms make no sense to me here. They are left below, for aeroquad compatability, but I have added these
// angle functions to calculate how much that axis is off center.
//
// Return values are in degrees, from -180 to 180.
//

float Accel::getXAngle(){
  return toDegrees(dataSmoothed[XAXIS]);
}

float Accel::getYAngle(){
  return toDegrees(dataSmoothed[YAXIS]);
}

float Accel::getZAngle(){
  return toDegrees(dataSmoothed[ZAXIS]);
}

float Accel::toDegrees(float value){
  return (value + 9.80665) * 360 / 19.6133 - 180;
}

///////////

// Rotation amount on a horizontal line between the left and right engines
// i.e. Is the left-right (Y) axis pointed up or down?
// Positive is right-side up, negative is left-side up
float Accel::getPitch(){
  return dataSmoothed[YAXIS];
}

// Rotation amount on a horizontal line drawn between the front and rear engines
// i.e. Is the forward (X) axis pointed up or down?
// Positive is down, negative is up
float Accel::getRoll(){
  return dataSmoothed[XAXIS];
}

// Force on a vertical line through the center of the aircraft
// i.e. How fast are we falling/climbing?
float Accel::getYaw(){
  return dataSmoothed[ZAXIS];
}

///////////

int Accel::getRawPitch(){
  return dataRaw[YAXIS];
}

int Accel::getRawRoll(){
  return dataRaw[XAXIS];
}

int Accel::getRawYaw(){
  return dataRaw[ZAXIS];
}

/////////////

float Accel::getSmoothFactor(){
  return _smoothFactor;
}

float Accel::getOneG(){
  return _oneG;
}
