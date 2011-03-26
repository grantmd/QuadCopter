/*
  Gyro.h - Library for interfacing with the ITG-3200 gyro (specifically, via the Sparkfun IMU Digital Combo Board: SEN-10121)
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
#ifndef Gyro_h
#define Gyro_h

#include "WProgram.h"
#include "Definitions.h"
#include "I2C.h"

class Gyro
{
  public:
    Gyro();
    void init();
    int getTemp();
    int getX();
    int getY();
    int getZ();
    
  private:
    I2C _i2c;
    int temp;
    int x;
    int y;
    int z;
};

#endif


