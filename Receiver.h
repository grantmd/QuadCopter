/*
  Receiver.h - Library for interfacing with an RC receiver
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
#ifndef Receiver_h
#define Receiver_h

#include "WProgram.h"
#include "Definitions.h"

class Receiver
{
  public:
    Receiver();
    
    void updateAll();
    int getChannel(byte channel);
    float getSmoothedChannel(byte channel);
    float getAngle(byte channel);
    
  private:
    int channels[6]; // Channel-to-pin assignments
    int readings[6]; // Current values for the channels
    float smoothed[6]; // Current smoothed values for the channels

    float _smoothFactor; // Smoothing for all channels
};

#endif
