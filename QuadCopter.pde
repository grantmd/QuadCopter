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
#include "Engines.h"

#ifdef SENSORS_ENABLED
#include "Gyro.h"
#include "Accel.h"
#endif

Engines engines;

#ifdef SENSORS_ENABLED
Gyro gyro;
Accel accel;
#endif

// Activity
byte red_led = 12; // System status
byte green_led = 13; // Motor activity

// Status
byte system_mode = 0; // 0: Unknown, 1: Auto, 2: Manual, 3: Engine Test, 4: Sensor Test

int pos = 0;
 
void setup(){
  Serial.begin(9600);
  Wire.begin(); // For the gyro and accel
  
  // Activity LEDs
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  
  digitalWrite(red_led, HIGH); // Indicate system is not ready
    
  engines.init();
  #ifdef SENSORS_ENABLED
  gyro.init();
  accel.init();
  #endif
  
  Serial.println("Setup complete");
  Serial.println("Choose mode (1: Auto, 2: Manual, 3: Engine Test, 4: Sensor Test)");
}

void loop(){
  if (Serial.available() > 0){
    // Wait for the buffer
    delay(40);
    
    // Construct a string out of the bytes available
    const int char_count = Serial.available();
    char input[char_count];
    
    for (int i=0; i<char_count; i++){
      input[i] = Serial.read();
    }
    
    // Convert string to int
    int input_int = atoi(input);
    
    // Set speed or mode
    if (system_mode == 2){
      engines.setThrottle(input_int);
    }
    else{
      system_mode = input_int;
      digitalWrite(red_led, LOW);
      
      Serial.print("Setting mode: ");
      Serial.println(system_mode, DEC);
    }
    
    // Prompt for next speed
    if (system_mode == 2){
      Serial.println("Enter throttle (0-1000):");
    }
  }
  else if (system_mode == 1){
    
    if (!engines.isArmed()) engines.arm();
    
    // Accelerate to max speed
    Serial.println("Accelerating");
    digitalWrite(green_led, HIGH);
    for(pos = 0; pos <= 1000; pos += 10){
      engines.setThrottle(pos);
      delay(500);
    }
    
    // Hold at max speed
    Serial.println("Holding at max speed");
    digitalWrite(green_led, LOW);
    delay(5000);
    
    // Decelerate to stop
    Serial.println("Decelerating");
    digitalWrite(green_led, HIGH);
    for(pos = 1000; pos >= 0; pos -= 10){
      engines.setThrottle(pos);
      delay(500);
    }
    
    // Hold at stop
    Serial.println("Holding at stop");
    digitalWrite(green_led, LOW);
    delay(5000);
  }
  else if (system_mode == 3){
    if (!engines.isArmed()) engines.arm();
    
    engines.setThrottle(100);
    digitalWrite(green_led, HIGH);
    delay(500);
    engines.setThrottle(0);
    digitalWrite(green_led, LOW);
    delay(500);
  }
  else if (system_mode == 4){
    
    #ifdef SENSORS_ENABLED
    gyro.updateAll();
    Serial.print("Gyro: ");
    Serial.print(gyro.getRoll());
    //Serial.print(", ");
    //Serial.print(gyro.getRawRoll());
    Serial.print(" | ");
    Serial.print(gyro.getPitch());
    //Serial.print(", ");
    //Serial.print(gyro.getRawPitch());
    Serial.print(" | ");
    Serial.print(gyro.getYaw());
    //Serial.print(", ");
    //Serial.print(gyro.getRawYaw());
    Serial.println();
    
    accel.updateAll();
    Serial.print("Accel: ");
    Serial.print(accel.getRoll());
    //Serial.print(", ");
    //Serial.print(accel.getRawRoll());
    Serial.print(" | ");
    Serial.print(accel.getPitch());
    //Serial.print(", ");
    //Serial.print(accel.getRawPitch());
    Serial.print(" | ");
    Serial.print(accel.getYaw());
    //Serial.print(", ");
    //Serial.print(accel.getRawYaw());
    Serial.println();
    #endif

    delay(100);
  }
  else if (millis() >= 10000 && system_mode != 2){
    Serial.println("Auto-entering Engine Test mode");
    system_mode = 3;
    digitalWrite(red_led, LOW);
  }
} 
