/*
  Engines.cpp - Library for controlling a set of Quadcopter engines (aka motors)
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
#include "Engines.h"

Engines::Engines(){  
  // Setup engines
  engines[LEFT_FRONT_MOTOR] = LEFT_FRONT_MOTOR_PIN;
  engines[RIGHT_FRONT_MOTOR] = RIGHT_FRONT_MOTOR_PIN;
  engines[LEFT_REAR_MOTOR] = LEFT_REAR_MOTOR_PIN;
  engines[RIGHT_REAR_MOTOR] = RIGHT_REAR_MOTOR_PIN;
}

void Engines::init(){
  //allStop();
}

void Engines::allStop(){
  throttle = 0;
  setAllSpeed(0);
}

void Engines::setEngineSpeed(byte engine, int speed){
  speed = constrain(speed, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  
  // Analog write supports commands from 0-255 => 0 - 100% duty cycle
  // Using 125-250 for motor setting 1000-2000
  analogWrite(engines[engine], speed / 8);
  
  engine_speeds[engine] = speed;
}

int Engines::getEngineSpeed(byte engine){
  return engine_speeds[engine];
}

void Engines::setAllSpeed(int speed){
  for (byte engine = 0; engine < ENGINE_COUNT; engine++){
    setEngineSpeed(engine, speed);
  }
}

// Increase/decrease throttle. Flight Control takes care of applying this to the engines
void Engines::setThrottle(int new_throttle){
  throttle = new_throttle;
}

int Engines::getThrottle(){
  return throttle;
}

void Engines::arm(byte method){
  if (_armed) return;
  
  switch (method){
    case 1:
      setAllSpeed(MAX_MOTOR_SPEED);
      break;
    default:
      setAllSpeed(MIN_MOTOR_SPEED);
  }

  _armed = true;
}

void Engines::disarm(){
  if (!_armed) return;
  
  setThrottle(0);
  allStop();
  
  _armed = false;
}

boolean Engines::isArmed(){
  return _armed;
}
