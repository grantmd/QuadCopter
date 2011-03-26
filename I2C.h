/*
  I2C.h - Convenience library for dealing with I2C-connected sensors
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
#ifndef I2C_h
#define I2C_h

#include "WProgram.h"

class I2C
{
  public:
    I2C();
    I2C(int);
    int getAddressFromDevice();
    boolean validateDevice();
    int writeSetting(byte, byte);
    int sendReadRequest(int);
    word readWord();
    byte readByte();
  private:
    int _address;
};

#endif


