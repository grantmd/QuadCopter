/*
  FlightControl.pde - Library for integrating sensor/engine/control data on my Quadcopter and deciding what to do
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

#include "PID.h"

PID levelRollPID = PID(4.0, 0.15, 0.0);
//PID levelRollGyroPID = PID(100.0, 0.0, -300.0);
PID levelPitchPID = PID(4.0, 0.15, 0.0);
//PID levelPitchGyroPID = PID(100.0, 0.0, -300.0);

void processFlightControl(){
  //
  // Can't really do anything without sensors/imu
  //
  
  #ifdef SENSORS_ENABLED
  float currentRoll = imu.getRoll();
  float currentPitch = imu.getPitch();
  
  //float rollRate = gyro.getRoll();
  //float pitchRate = gyro.getPitch();
  
  // Level flight assumed
  float targetRoll = 0;
  float targetPitch = 0;
  
  //
  // Don't adjust pitch/roll if we are not armed!
  //
  
  if (engines.isArmed()){
    float G_Dt = deltaTime / 1000000.0;
    
    // Negative values mean the right side is up
    float rollAdjust = levelRollPID.updatePID(targetRoll, currentRoll, G_Dt);
    //float rollGyroAdjust = levelRollGyroPID.updatePID(targetRoll, rollRate, G_Dt);
    
    // Positive values mean the frontend is up
    float pitchAdjust = levelPitchPID.updatePID(targetPitch, currentPitch, G_Dt);
    //float pitchGyroAdjust = levelPitchGyroPID.updatePID(targetRoll, pitchRate, G_Dt);
    
    // Apply offsets to all motors evenly to ensure we pivot on the center
    int throttle = engines.getThrottle() + MIN_MOTOR_SPEED;
    if (throttle > MIN_MOTOR_SPEED){
      engines.setEngineSpeed(LEFT_FRONT_MOTOR, throttle - rollAdjust - pitchAdjust);
      engines.setEngineSpeed(RIGHT_FRONT_MOTOR, throttle + rollAdjust - pitchAdjust);
      engines.setEngineSpeed(LEFT_REAR_MOTOR, throttle - rollAdjust + pitchAdjust);
      engines.setEngineSpeed(RIGHT_REAR_MOTOR, throttle + rollAdjust + pitchAdjust);
    }
    else{
      engines.setAllSpeed(0);
    }
    
    /*Serial.print(targetPitch);
    Serial.print(" | ");
    Serial.print(currentPitch);
    Serial.print(" | ");
    Serial.println(pitchAdjust);*/
    //delay(100);
  }
  
  #endif
}
