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

#include "WProgram.h"
#include "IMU.h"

IMU::IMU(){
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    data[axis] = 0.0;
  }
  
  _dt = 0.01;
  _timeConstant = 0.98;
}

// Update the filter based on most recent values
void IMU::update(float gx, float gy, float gz, float ax, float ay, float az){
  updateAxis(ROLL, gx, ay);
  updateAxis(PITCH, gy, ax);
  updateAxis(YAW, gz, az);
}

// Get filtered pitch roll
float IMU::getRoll(){
  return data[ROLL];
}

// Get filtered pitch angle
float IMU::getPitch(){
  return data[PITCH];
}

// This doesn't work!
float IMU::getHeading(){
  return data[YAW];
}

// Update an axis using the complementary filter
void IMU::updateAxis(byte axis, float gyro, float accel){
  data[axis] = (_timeConstant * (data[axis] + (gyro * _dt))) + ((1 - _timeConstant) * accel);
}
