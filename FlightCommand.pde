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
  
  // Which mode?
  if (systemMode == 0){
    return processReceiverCommands();
  }
  
  return processAutoPilot();

}

void processReceiverCommands(){
  // Arm the engines by putting the left stick in the lower-right corner
  if (receiver.getSmoothedChannel(THROTTLE_CHANNEL) < 1100 && receiver.getSmoothedChannel(YAW_CHANNEL) > 1850){
    engines.arm(0);
    baro.setGroundAltitude();
  }
  
  // Disarm the engines by putting the left stick in the lower-left corner
  if (receiver.getSmoothedChannel(THROTTLE_CHANNEL) < 1100 && receiver.getSmoothedChannel(YAW_CHANNEL) < 1100){
    engines.disarm();
  }
  
  // Process throttle
  engines.setThrottle(receiver.getSmoothedChannel(THROTTLE_CHANNEL)-MIN_MOTOR_SPEED); // Engines expect throttle to be 0-based
}

void processAutoPilot(){
  if (commandTime == 0) commandTime = currentTime + 5000000;
  
  int throttle = engines.getThrottle();
  
  //
  // Auto-arm after 10s
  //
  
  if (!engines.isArmed() && currentTime >= commandTime){
    engines.arm(0);
    baro.setGroundAltitude();
    
    isClimbing = true;
    
    // Start in 5s
    commandTime = currentTime + 5000000;
  }
  else{
    //
    // Every 0.8s, go up/down 10 throttle units
    //
    
    if (currentTime >= commandTime){
      commandTime = currentTime + 80000;
      
      if (isClimbing){
        engines.setThrottle(throttle+10);
        
        if (throttle >= 480){
          isClimbing = false;
          
          // If in Auto mode, begin descending in 20s
          if (systemMode == 1){
            commandTime = currentTime + 20000000;
            isDescending = true;
          }
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
