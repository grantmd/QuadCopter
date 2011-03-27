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
#include <Servo.h>
#include "Definitions.h"
#include "Engines.h"
#include "I2C.h"
#include "Accel.h"
#include "Gyro.h"

Engines engines;
Gyro gyro;
Accel accel;

// Activity
byte red_led = 12; // System status
byte green_led = 13; // Motor activity

// Status
int system_mode = 0; // 0: Unknown, 1: Auto, 2: Manual, 3: Engine Test, 4: Sensor Test

int pos = 0;
 
void setup(){
  Serial.begin(9600);
  
  // Activity LEDs
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  
  digitalWrite(red_led, HIGH); // Indicate system is not ready
    
  engines.init();
  gyro.init();
  accel.init();
  
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
      Serial.println(system_mode);
    }
    
    // Prompt for next speed
    if (system_mode == 2){
      Serial.println("Enter throttle (0-1000):");
    }
  }
  else if (system_mode == 1){
    
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
    engines.setThrottle(100);
    digitalWrite(green_led, HIGH);
    delay(250);
    engines.setThrottle(0);
    digitalWrite(green_led, LOW);
    delay(250);
  }
  else if (system_mode == 4){
    gyro.updateAll();
    
    Serial.print("Gyro: ");
    Serial.print(gyro.getRoll());
    Serial.print(", ");
    Serial.print(gyro.getPitch());
    Serial.print(", ");
    Serial.println(gyro.getYaw());

    delay(100);
  }
  else if (millis() >= 10000){
    Serial.println("Auto-entering Engine Test mode");
    system_mode = 3;
    digitalWrite(red_led, LOW);
  }
} 
