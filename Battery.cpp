/*
  Battery.cpp - Library for reading remaining voltage from an Arduino circuit
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

#include "WProgram.h"
#include "Definitions.h"
#include "Battery.h"

Battery::Battery(){
  _batteryScaleFactor = ((BATTERY_AREF / 1024.0) * ((BATTERY_R1 + BATTERY_R2) / BATTERY_R2));
}

void Battery::init(){
  analogReference(DEFAULT);
  _alarmTime = millis();
  _alarmLight = 0;
}

void Battery::measure(){
  long currentTime = micros();
  _batteryVoltage = (analogRead(BATTERY_PIN) * _batteryScaleFactor) + BATTERY_DIODE;
  
  if (_batteryVoltage <= ALARM_VOLTAGE){
    if (currentTime > _alarmTime){
      currentTime = _alarmTime + ALARM_RATE;
      if (_alarmLight == 1){
        digitalWrite(RED_LED, LOW);
        _alarmLight = 0;
      }
      else{
        digitalWrite(RED_LED, HIGH);
        _alarmLight = 1;
      }
    }
  }
}

float Battery::getData(){
  return _batteryVoltage;
}
