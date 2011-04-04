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

#ifdef SENSORS_ENABLED
#include "Gyro.h"
#include "Accel.h"
#include "IMU.h"
Gyro gyro;
Accel accel;
IMU imu;
#endif

#include "Engines.h"
Engines engines;

// Activity
byte red_led = 12; // System status
byte green_led = 13; // Motor activity

// Status
byte system_mode = 0; // 0: Unknown, 1: Auto, 2: Manual, 3: Engine Test, 4: Sensor Test

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
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  
  digitalWrite(red_led, HIGH); // Indicate system is not ready
  
  //
  // TODO: Load from eeprom
  //
  
  //
  // Run inits
  //
  engines.init();
  #ifdef SENSORS_ENABLED
  gyro.init();
  accel.init();
  #endif
  
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
  
  #ifdef SENSORS_ENABLED
  gyro.updateAll();
  accel.updateAll();
  imu.update(deltaTime/1000, gyro.getRoll(), gyro.getPitch(), gyro.getYaw(), accel.getXAngle(), accel.getYAngle(), accel.getZAngle());
  #endif

  //
  // Decide what to do and do it (flight control)
  //
  
  processFlightControl();
  
  //
  // TODO: Take commands from.... where? (flight command)
  //
  
  //
  // Read serial commands and set them/reply
  //
  
  #ifdef SERIALCONTROL_ENABLED
  if (currentTime > serialTime){
    serialTime = currentTime + SERIAL_RATE;
    
    readSerialCommand();
    sendSerialTelemetry();
  }
  #endif
} 
