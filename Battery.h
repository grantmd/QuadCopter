/*
  Battery.h - Library for reading remaining voltage from an Arduino circuit
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
#ifndef Battery_h
#define Battery_h

#include "WProgram.h"
#include "Definitions.h"

class Battery
{
  public:
    Battery();
    void init();

    void measure();

    float getData();
    
  private:
    float _batteryScaleFactor;
    float _batteryVoltage;
    
    long _alarmTime;
    byte _alarmLight;
};

#endif
