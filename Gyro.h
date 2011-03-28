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

class Gyro : public I2C
{
  public:
    Gyro();
    void init();
    void autoZero();
    
    void updateAll();
    
    // The raw values from the sensor
    int getRawRoll();
    int getRawPitch();
    int getRawYaw();
    
    // Smoothed/compensated values
    int getTemp();
    int getRoll();
    int getPitch();
    int getYaw();
    
    void sleep();
    void unsleep();
    
  private:
    I2C _i2c;
    
    int temp; // Most recent temp (converted to degrees F)
    int dataRaw[3]; // Raw and unfiltered gyro data
    float dataSmoothed[3]; // Smoothed gyro data
    int zero[3]; // Zero points for the gyro axes
    
    float _scaleFactor; // How to convert raw sensor data to SI units
    float _smoothFactor; // 1.0 to not smooth, otherwise adjust as necessary

    long int _lastMeasureTime;
    boolean _sleeping;
};

#endif


