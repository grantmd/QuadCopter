/*
  Accel.cpp - Library for interfacing with the ADXL345 accelerometer (specifically, via the Sparkfun IMU Digital Combo Board: SEN-10121)
  Created by Myles Grant <myles@mylesgrant.com>
  See also: https://github.com/grantmd/QuadCopter
  
  Datasheet: http://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
  
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
#include "EEPROM_lib.h"

Accel::Accel() : I2C(){
  _smoothFactor = 0.8;

  // From: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231185714/30
  gConstant[XAXIS] = 2.0 / float(MAX_ACCEL_ROLL - MIN_ACCEL_ROLL);
  gB[XAXIS] = 1 - gConstant[XAXIS] * MAX_ACCEL_ROLL;
  
  gConstant[YAXIS] = 2.0 / float(MAX_ACCEL_PITCH - MIN_ACCEL_PITCH);
  gB[YAXIS] = 1 - gConstant[YAXIS] * MAX_ACCEL_PITCH;
  
  gConstant[ZAXIS] = 2.0 / float(MAX_ACCEL_YAW - MIN_ACCEL_YAW);
  gB[ZAXIS] = 1 - gConstant[ZAXIS] * MAX_ACCEL_YAW;
}

void Accel::init(){
  //Serial.println("Initing Accel");
  setAddress(ACCEL_ADDR);
  
  if (!getAddressFromDevice()){
    //Serial.println("ACCEL NOT CONNECTED!");
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
}

// Calculate zero for all 3 axis, storing it for later measurements
// This assumes your platform is truly level!
// See also: http://www.freescale.com/files/sensors/doc/app_note/AN3447.pdf
void Accel::autoZero(){
  // Take 50 measurements of all 3 axis, find the median, that's our zero-point
  // Why 50? Because that's what the aeroquad project does
  byte loopCount = 50;
  
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
}

// Updates all raw measurements from the accelerometer
void Accel::updateAll(){
  sendReadRequest(0x32);
  requestBytes(6);

  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
     dataRaw[axis] = zero[axis] - readNextWordFlip();
     dataSmoothed[axis] = filterSmooth(gConstant[axis] * dataRaw[axis] + gB[axis], dataSmoothed[axis], _smoothFactor);
  }
}

///////////

//
// Now, either I am doing something wrong, there is something wrong with my board, or I don't understand how these things work...
// But the pitch/roll/yaw terms make no sense to me here. They are left below, for aeroquad compatability, but I have added these
// angle functions to calculate how much that axis is off center.
//
// See also: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231185714/30 and http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
//
// Return values are in degrees, from -180 to 180.
//

// Pitch!
float Accel::getXAngle(){
  return atan2(dataSmoothed[XAXIS], sqrt( sq(dataSmoothed[YAXIS]) + sq(dataSmoothed[ZAXIS]) ) ) * 57.296;
}

// Roll!
float Accel::getYAngle(){
  return atan2( dataSmoothed[YAXIS], sqrt( sq(dataSmoothed[XAXIS]) + sq(dataSmoothed[ZAXIS]) ) ) * 57.296;
}

// Yaw/Theta!
float Accel::getZAngle(){
  return atan2( sqrt( sq(dataSmoothed[XAXIS]) + sq(dataSmoothed[YAXIS]) ), dataSmoothed[ZAXIS] ) * 57.296;
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
