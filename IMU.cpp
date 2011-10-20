/*
  IMU.cpp - Library for combining my accel and gyro data into one nice set of headings/angles
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

//
// This is a Complementary Filter!
// http://web.mit.edu/scolton/www/filter.pdf
//
// Maybe in the future it should be a Kalman filter:
// http://tom.pycke.be/mav/71/kalman-filtering-of-imu-data
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1248889032/35
//

#include "WProgram.h"
#include "IMU.h"

IMU::IMU(){
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    data[axis] = 0.0;
  }
  
  _a = 0.96;
  _b = 1 - _a;
  _dtGyro = 0.1; // Update rate of gyros: 1kHz (in millis)
}

// Update the filter based on most recent values
// dT is in milliseconds
// g* is gyro rotation rate in degrees/s
// a* is current angle in degrees
// heading is magnetometer heading in degrees
void IMU::update(int dT, float gx, float gy, float gz, float ax, float ay, float az, float heading){
  updateAxis(ROLL, dT, gx, ax);
  updateAxis(PITCH, dT, gy, ay);
  updateAxis(YAW, dT, gz, heading);
}

// Get filtered roll angle
// Positive left, negative right
float IMU::getRoll(){
  return data[ROLL];
}

// Get filtered pitch angle
// Positive forward, negative backward
float IMU::getPitch(){
  return data[PITCH];
}

// Our absolute compass direction in degrees
float IMU::getHeading(){
  return data[YAW];
}

// Update an axis using the complementary filter
// dT is in millis
// gyro is gyro rotation rate in degrees/s
// accel is current angle in degrees
void IMU::updateAxis(byte axis, int dT, float gyro, float accel){
  data[axis] = (_a * (data[axis] + (gyro * dT * _dtGyro))) + (_b * accel);
}
