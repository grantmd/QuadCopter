/*
  EEPROM.h - Library for making it easier to save/load values to the Arduino's EEPROM for my QuadCopter
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
#ifndef EEPROM_h
#define EEPROM_h

#include "WProgram.h"
#include "Definitions.h"

byte eeprom_read(int);
void eeprom_write(int, byte);

void eeprom_write_all();
void eeprom_read_all();

#endif
