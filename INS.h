/*
  INS.h - Library for implementing an Inertial Navigation System for my Quadcopter
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
#ifndef INS_h
#define INS_h

#include "WProgram.h"
#include "Definitions.h"

class INS
{
  public:
    INS();
    void init();
    
    void update(int, float, float, float, float);
    
    int getXPosition();
    int getYPosition();
    int getZPosition();
    
  private:
    void updateAxis(byte, int, float);
    void movementEndCheck(byte);
  
    float _acceleration[3];
    float _velocity[3];
    int _position[3];
    int _zeroCount[3];
};

#endif
