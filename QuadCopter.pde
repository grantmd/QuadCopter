/*
  Quadcopter!!!!
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

// Include libs
#include <Wire.h>
#include "Definitions.h"
#include <EEPROM.h>
#include "EEPROM_lib.h"

#include "Gyro.h"
#include "Accel.h"
#include "Baro.h"
#include "Mag.h"
#include "IMU.h"
#include "INS.h"

#include "PID.h"

Gyro gyro;
Accel accel;
Baro baro;
Mag mag;
IMU imu;
INS ins;

#include "Engines.h"
Engines engines;

#include "Receiver.h"
Receiver receiver;

#include "Battery.h"
Battery battery;

// Status
byte systemMode = 0; // 0: Manual, 1: Auto, 2: PID Test
byte activityLight = 0; // 0: Off, 1: On

unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long deltaTime = 0;
unsigned long serialTime = 0;
unsigned long activityTime = 0;

void setup(){
  Serial.begin(115200);
  Wire.begin(); // For the gyro and accel
  
  //
  // Activity LEDs
  //
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  //
  // Run inits
  //

  battery.init();
  engines.init();

  gyro.init();
  accel.init();
  baro.init();
  mag.init();
  
  //
  // It's go time
  //
  
  previousTime = micros();
}

void loop(){ 
  //
  // Measure loop rate
  //
  currentTime = micros();
  deltaTime = currentTime - previousTime;
  previousTime = currentTime;
  
  //
  // Get the latest data
  //
  
  gyro.updateAll();
  accel.updateAll();
  baro.measure();
  mag.updateAll(accel.getYAngle(), accel.getXAngle());
  imu.update(deltaTime/1000, gyro.getRoll(), gyro.getPitch(), gyro.getYaw(), accel.getYAngle(), accel.getXAngle(), accel.getZAngle(), mag.getHeadingDegrees());
  ins.update(deltaTime/1000, accel.getRoll(), accel.getPitch(), accel.getYaw(), imu.getHeading());
  receiver.updateAll();
  battery.measure();

  //
  // Decide what to do and do it (flight control)
  //
  
  processFlightControl();
  
  //
  // Take commands from.... where? (flight command)
  //
  
  processFlightCommand();
  
  //
  // Read serial commands and set them/reply
  //
  
  if (currentTime > serialTime){
    serialTime = currentTime + SERIAL_RATE;
    
    readSerialCommand();
    sendSerialTelemetry();
  }
  
  if (currentTime > activityTime){
    activityTime = currentTime + ACTIVITY_RATE;
    
    if (activityLight == 1){
      digitalWrite(GREEN_LED, LOW);
      activityLight = 0;
    }
    else{
      digitalWrite(GREEN_LED, HIGH);
      activityLight = 1;
    }
  }
}
