/*
  PID.cpp - Library for implementing a PID control loop. Used to ensure engines don't overshoot when reaching target roll/pitch.
  Created by Myles Grant <myles@mylesgrant.com>
  Based on: http://www.arduino.cc/playground/Main/BarebonesPIDForEspresso#pid
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
#include "PID.h"

PID::PID(){
  iState = 0;
  last = 0;
  
  pgain = 0;
  igain = 0;
  dgain = 0;
}

PID::PID(float p, float i, float d){
  PID();
  
  pgain = p;
  igain = i;
  dgain = d;
}

// get the P gain 
float PID::getP(){
  return pgain;
}

// get the I gain
float PID::getI(){
  return igain;
}

// get the D gain
float PID::getD(){
  return dgain;
}

// set the P gain and store it to eeprom
void PID::setP(float p){
  pgain = p; 
  //writeFloat(p, pgainAddress);
}

// set the I gain and store it to eeprom
void PID::setI(float i){
  igain = i; 
  //writeFloat(i, igainAddress);
}

// set the D gain and store it to eeprom
void PID::setD(float d){
  dgain = d; 
  //writeFloat(d, dgainAddress);
}

float PID::updatePID(float target, float cur, float deltaTime){
  // these local variables can be factored out if memory is an issue, 
  // but they make it more readable
  float error;
  float windupGuard;

  // determine how badly we are doing
  error = target - cur;

  // the pTerm is the view from now, the pgain judges 
  // how much we care about error at this instant.
  pTerm = pgain * error;

  // iState keeps changing over time; it's 
  // overall "performance" over time, or accumulated error
  iState += error * deltaTime;

  // to prevent the iTerm getting huge despite lots of 
  //  error, we use a "windup guard" 
  // (this happens when the machine is first turned on and
  // it cant help be cold despite its best efforts)

  // not necessary, but this makes windup guard values 
  // relative to the current iGain
  windupGuard = WINDUP_GUARD_GAIN / igain;  

  if (iState > windupGuard) 
    iState = windupGuard;
  else if (iState < -windupGuard) 
    iState = -windupGuard;
  iTerm = igain * iState;

  // the dTerm, the difference between the temperature now
  //  and our last reading, indicated the "speed," 
  // how quickly the temp is changing. (aka. Differential)
  dTerm = (dgain * (cur - last)) / deltaTime;

  // now that we've use lastTemp, put the current temp in
  // our pocket until for the next round
  last = cur;

  // the magic feedback bit
  return pTerm + iTerm - dTerm;
}

void PID::resetError(){
  iState = 0;
}
