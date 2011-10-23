/*
  Mag.h - Library for interfacing with the HMC5883L Triple Axis Magnetometer from Sparkfun:
  http://www.sparkfun.com/products/10530
  http://www.sparkfun.com/products/9441 - HMC5843 works as well
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
#ifndef Mag_h
#define Mag_h

#include "WProgram.h"
#include "Definitions.h"
#include "I2C.h"

class Mag : public I2C
{
  public:
    Mag();
    void init();
    
    void updateAll(float roll, float pitch);

    int getRaw(byte axis); // The raw values from the sensor

    float getHeading();
    float getHeadingDegrees();
    
  private:
    I2C _i2c;

    int dataRaw[3]; // Raw and unfiltered data
    float _heading; // tilt-compensated heading
    float _scale;
};

#endif
