/*
  EEPROM_lib.cpp - Library for making it easier to save/load values to the Arduino's EEPROM for my QuadCopter
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
#include "Definitions.h"
#include <EEPROM.h>
#include "EEPROM_lib.h"

////////////

// Read a single value from eeprom
byte eeprom_read(int address){
  return EEPROM.read(address);
}

// Read 4 bytes from eeprom and make it a float
float eeprom_read_float(int address){
  // a union is a datatype that allows us to write to it in bytes and read back as a float
  // (and vice versa)
  union floatStore{
    byte floatByte[4];
    float floatVal;
  } floatOut;

  for (int i=0; i<4; i++){
    floatOut.floatByte[i] = eeprom_read(address + i);
  }
  
  return floatOut.floatVal;
}

// Read 2 bytes from eeprom and make it an int
int eeprom_read_int(int address){
  // a union is a datatype that allows us to write to it in bytes and read back as an int
  // (and vice versa)
  union intStore{
    byte intByte[2];
    int intVal;
  } intOut;

  for (int i=0; i<2; i++){
    intOut.intByte[i] = eeprom_read(address + i);
  }
  
  return intOut.intVal;
}

// Write a single value to eeprom
void eeprom_write(int address, byte value){
  EEPROM.write(address, value);
}

// Turn a float into 4 bytes and write them to eeprom
void eeprom_write(int address, float value){
  // a union is a datatype that allows us to write to it in bytes and read back as a float
  // (and vice versa)
  union floatStore{
    byte floatByte[4];
    float floatVal;
  } floatIn;

  floatIn.floatVal = value;

  for (int i=0; i<4; i++){
    eeprom_write(address + i, floatIn.floatByte[i]);
  }
}

// Turn an int into 2 bytes and write them to eeprom
void eeprom_write(int address, int value){
  // a union is a datatype that allows us to write to it in bytes and read back as an int
  // (and vice versa)
  union intStore{
    byte intByte[2];
    int intVal;
  } intIn;
  
  intIn.intVal = value;

  for (int i=0; i<2; i++){
    eeprom_write(address + i, intIn.intByte[i]);
  }
}

//////////////

// Write everything we care about to eeprom
void eeprom_write_all(){
}

// Reload everything we care about from eeprom
void eeprom_read_all(){
}
