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

#include <Servo.h>
#include "WProgram.h"
#include "Definitions.h"
#include "Engines.h"

Engines::Engines(){  
  // Setup engines
  engines[0].attach(3);
  engines[1].attach(9);
  engines[2].attach(10);
  engines[3].attach(11);
}

void Engines::init(){
  allStop();
  
  Serial.println("Engines ready");
}

void Engines::allStop(){
  Serial.println("All stop");
  throttle = 0;
  setAllSpeed(MIN_MOTOR_SPEED);
}

void Engines::setEngineSpeed(byte engine, int speed){
  speed = constrain(speed, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  engines[engine].writeMicroseconds(speed);
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

// Increase/decrease throttle
void Engines::setThrottle(int new_throttle){
  int delta = new_throttle - throttle;
  throttle = new_throttle;
  
  for (byte engine = 0; engine < ENGINE_COUNT; engine++){
    setEngineSpeed(engine, getEngineSpeed(engine)+delta);
  }
}

void Engines::arm(){
  if (_armed) return;
  
  Serial.print("Arming engines... ");
  setThrottle(50);
  delay(1000);
  Serial.println("Armed");
  
  _armed = true;
}

void Engines::disarm(){
  if (!_armed) return;
  
  Serial.print("Disarming engines... ");
  setThrottle(0);
  delay(1000);
  Serial.println("Disarmed");
  
  _armed = false;
}

boolean Engines::isArmed(){
  return _armed;
}
