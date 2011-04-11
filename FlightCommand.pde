/*
  FlightCommand.pde - Library for giving commands to my Quadcopter (aka autopilot)
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

boolean isClimbing = false;
boolean isDescending = false;

unsigned long commandTime = 0;

void processFlightCommand(){
  return;
  
  //
  // Auto-arm after 30s
  //
  
  if (!engines.isArmed() && currentTime >= 30000000){
    engines.arm(0);
    isClimbing = true;
  }
  else{
    //
    // Every 2s, go up/down 10 throttle units
    //
    
    if (currentTime > commandTime){
      commandTime = currentTime + 2000000;
      
      int throttle = engines.getThrottle();
      
      if (isClimbing){
        engines.setThrottle(throttle+10);
        
        if (throttle >= 190){
          isClimbing = false;
          isDescending = true;
        }
      }
      else if (isDescending){
        engines.setThrottle(throttle-10);
        
        if (throttle <= 10){
          isDescending = false;
          engines.allStop();
        }
      } 
    }
  }
}
