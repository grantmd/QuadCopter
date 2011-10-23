/*
  Global definitions for my quadcopter. Holds constants and config flags for either globally
    useful data or things I might want to change easily. There may be other definitions elsewhere
    in the codebase!
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

// Global definitions
#define ENGINE_COUNT 4
// Speeds are in microseconds, not degrees, for greater control
#define MIN_MOTOR_SPEED 1000
#define MAX_MOTOR_SPEED 2000

#define LEFT_FRONT_MOTOR_PIN 9
#define RIGHT_FRONT_MOTOR_PIN 10
#define LEFT_REAR_MOTOR_PIN 3
#define RIGHT_REAR_MOTOR_PIN 11

#define LEFT_FRONT_MOTOR 0
#define RIGHT_FRONT_MOTOR  1
#define LEFT_REAR_MOTOR 2
#define RIGHT_REAR_MOTOR 3

#define GYRO_ADDR 0x68
#define ACCEL_ADDR 0x53
#define BARO_ADDR 0x77
#define MAG_ADDR 0x1E

#define ROLL 0
#define PITCH 1
#define YAW 2

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

#define VERSION 2.3 // Emulate Aeroquad

#define SERIAL_RATE 1000 // How long between running the serial code, in microseconds?

// Activity
#define RED_LED 31 // Battery alarm
#define YELLOW_LED 12 // Flight mode
#define GREEN_LED 13 // Loop is running
#define ACTIVITY_RATE 500000 // How long between activity light blinks, in microseconds?
#define ALARM_RATE 250000 // How long between activity light blinks, in microseconds?

// Measured raw accel values by putting the quadcopter on all 6 sides
#define MAX_ACCEL_ROLL 269
#define MAX_ACCEL_PITCH 268
#define MAX_ACCEL_YAW 0
  
#define MIN_ACCEL_ROLL -271
#define MIN_ACCEL_PITCH -260
#define MIN_ACCEL_YAW 510

// Receiver
#define CHANNELS 6
#define THROTTLE_CHANNEL 0
#define YAW_CHANNEL 1
#define PITCH_CHANNEL 2
#define ROLL_CHANNEL 3
#define GEAR_CHANNEL 4
#define AUX_CHANNEL 5

// Battery monitor
#define BATTERY_PIN 0 // This is the same on every arduino
#define BATTERY_R1 14560.0 // 15k resistor measured with a multimeter, must be a float!
#define BATTERY_R2 7390.0 // 7.5k resistor measured with a multimeter, must be a float!
#define BATTERY_AREF 5.0 // Arduino Mega runs at 5v
#define BATTERY_DIODE 0.9 // On-board diode, measured with a multimeter
#define ALARM_VOLTAGE 9.0
