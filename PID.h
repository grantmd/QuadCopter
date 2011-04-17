/*
  PID.h - Library for implementing a PID control loop. Used to ensure engines don't overshoot when reaching target roll/pitch.
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
#ifndef PID_h
#define PID_h

#include "WProgram.h"
#include "Definitions.h"

#define WINDUP_GUARD_GAIN 500.0

class PID
{
  public:
    PID();
    PID(float, float, float);
    
    float getP();
    float getI();
    float getD();
    
    void setP(float);
    void setI(float);
    void setD(float);
    
    float updatePID(float, float, float);
    
    void resetError();
  
  private:
    float pgain, igain, dgain; 
    float pTerm, iTerm, dTerm;
    
    float iState;
    float last;
};

#endif
