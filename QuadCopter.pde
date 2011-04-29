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
#include "EEPROM.h"

#include "Gyro.h"
#include "Accel.h"
#include "IMU.h"

#include "PID.h"

Gyro gyro;
Accel accel;
IMU imu;

#include "Engines.h"
Engines engines;

// Status
byte systemMode = 0; // 0: Manual, 1: Auto, 2: PID Test

int pos = 0;
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long deltaTime = 0;
unsigned long serialTime = 0;

void setup(){
  Serial.begin(115200);
  Wire.begin(); // For the gyro and accel
  
  //
  // Activity LEDs
  //
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  //
  // Run inits
  //
  engines.init();
  gyro.init();
  accel.init();
  
  //
  // It's go time
  //
  
  previousTime = micros();
}

void loop(){
  digitalWrite(GREEN_LED, HIGH);
    
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
  imu.update(deltaTime/1000, gyro.getRoll(), gyro.getPitch(), gyro.getYaw(), accel.getYAngle(), accel.getXAngle(), accel.getZAngle());

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
  
  digitalWrite(GREEN_LED, LOW);
}
