/*
  INS.cpp - Library for implementing an Inertial Navigation System for my Quadcopter
   i.e. a dead-reckoning system for obtaining current position based on the IMU data
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
#include "INS.h"

//
// This looks good: http://www.freescale.com/files/sensors/doc/app_note/AN3397.pdf
//

INS::INS(){
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    _velocity[axis] = 0.0;
    _acceleration[axis] = 0.0;
    _position[axis] = 0;
  }
}

void INS::update(int dT, float x_accel, float y_accel, float z_accel, float heading){
  updateAxis(XAXIS, dT, x_accel);
  updateAxis(YAXIS, dT, y_accel);
  updateAxis(ZAXIS, dT, z_accel);
}

void INS::updateAxis(byte axis, int dT, float accel){
  // first integration
  float velocity = _velocity[axis] + _acceleration[axis] + ((accel - _acceleration[axis]) / 2);

  // second integration
  _position[axis] = _position[axis] + _velocity[axis] + ((velocity - _velocity[axis]) / 2);
  
  // Store for next time
  _velocity[axis] = velocity;
  _acceleration[axis] = accel;
}

///////////

// I suppose these are in meters?
int INS::getXPosition(){
  return _position[XAXIS];
}

int INS::getYPosition(){
  return _position[YAXIS];
}

int INS::getZPosition(){
  return _position[ZAXIS];
}
