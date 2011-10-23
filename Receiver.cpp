/*
  Receiver.cpp - Library for interfacing with an RC receiver
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
#include "Receiver.h"
#include "Utils.h"

Receiver::Receiver(){
  // These may seem arbitrary, but they are what the AeroQuad software uses,
  // so I am using the same, for consistency
  //
  // However, for some reason that I have not yet discovered, Yaw and Roll seem to be swapped from what my receiver is labeled
  channels[THROTTLE_CHANNEL] = 4; // Throttle
  channels[ROLL_CHANNEL] = 2; // Rudd. / Roll
  channels[PITCH_CHANNEL] = 5; // Elev. / Pitch
  channels[YAW_CHANNEL] = 6; // Aile / Yaw
  channels[GEAR_CHANNEL] = 7; // Gear
  channels[AUX_CHANNEL] = 8; // Aux / Flt Mode
  
  int i;
  // Assign all pins for reading
  for (i=0; i<CHANNELS; i++){
    pinMode(channels[i], INPUT);
  }

  // Init the readings at something sensible
  for (i=0; i<CHANNELS; i++){
    smoothed[i] = (i == THROTTLE_CHANNEL) ? 1000 : 1500;
    readings[i] = smoothed[i];
  }

  _smoothFactor = 1.0;
}

void Receiver::updateAll(){
  int temp;
  int i;
  
  // Read all the channels. Worst case, this will take 180us
  for (i=0; i<CHANNELS; i++){
    temp = pulseIn(channels[i], HIGH, 20000); // Attempt to read a pulse for 20us
    if (temp != 0){ // A value of 0 means that the read timed out, so we keep our previous value
      smoothed[i] = filterSmooth(temp, readings[i], _smoothFactor); // Apply smoothing
      readings[i] = temp;
    }
  }
}

// TODO: Apply some sort of centering to these, since the offsets differ from receiver to receiver
int Receiver::getChannel(byte channel){
  return readings[channel];
}

float Receiver::getSmoothedChannel(byte channel){
  return smoothed[channel];
}

float Receiver::getAngle(byte channel){
  // Scale 1000-2000 usecs to -45 to 45 degrees
  return (0.09 * smoothed[channel]) - 135;
}
