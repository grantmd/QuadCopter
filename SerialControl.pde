/*
  SerialControl.pde - Library for controlling my Quadcopter via the serial connection
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

//
// NB: We follow the AeroQuad serial control protocol: http://aeroquad.com/wiki/index.php/AeroQuad_Serial_Commands
// Interop FTW.
//

byte _queryType;

void readSerialCommand(){
  // Check for serial message
  if (Serial.available()){
    _queryType = Serial.read(); // Read the command first

    switch (_queryType){
      case 'A': // TODO: Receive roll and pitch gyro PID
        //readSerialPID(ROLL);
        //readSerialPID(PITCH);
        //float minAcro = readFloatSerial();
        break;
      case 'C': // TODO: Receive yaw PID
        /*readSerialPID(YAW);
        readSerialPID(HEADING);
        headingHoldConfig = readFloatSerial();
        heading = 0;
        relativeHeading = 0;
        headingHold = 0;*/
        break;
      case 'E': // TODO: Receive roll and pitch auto level PID
        readSerialPID(levelRollPID);
        readSerialPID(levelPitchPID);
        //readSerialPID(LEVELGYROROLL);
        //readSerialPID(LEVELGYROPITCH);
        //windupGuard = readFloatSerial(); // defaults found in setup() of AeroQuad.pde
        break;
      case 'G': // Receive auto level configuration
        break;
      case 'I': // Receive altitude hold PID
        break;
      case 'K': // Receive data filtering values
        break;
      case 'M': // Receive transmitter smoothing values
        break;
      case 'O': // Receive transmitter calibration values
        break;
      case 'W': // Write all user configurable values to EEPROM
        eeprom_write_all();
        break;
      case 'Y': // Initialize EEPROM with default values
        eeprom_read_all();
        break;
      case '1': // Calibrate ESCS's by setting Throttle high on all channels
        engines.disarm();
        engines.arm(1);
        break;
      case '2': // Calibrate ESC's by setting Throttle low on all channels
        engines.disarm();
        engines.arm(2);
        break;
      case '3': // Test ESC calibration
        engines.disarm();
        engines.setThrottle(constrain(readFloatSerial(), 1000, 1200));
        break;
      case '4': // Turn off ESC calibration
        engines.disarm();
        break;
      case '5': // Send individual motor commands (motor, command)
        engines.disarm();
        engines.setThrottle(constrain(readFloatSerial(), 1000, 1200));
        // HOW DOES THIS DIFFER FROM 3!?
        break;
      case 'a': // fast telemetry transfer
        // IGNORED
        break;
      case 'b': // calibrate gyros
        gyro.autoZero();
        break;
      case 'c': // calibrate accels
        accel.autoZero();
        break;
      case 'd': // send aref
        // IGNORED
        break;
      case 'f': // calibrate magnetometer
        // IGNORED
        break;
      case '~': // read Camera values 
        // IGNORED
        break;
      
      // The following modes are my own
      case '$': // Set throttle
        engines.setThrottle(readIntSerial());
        break;
      case 's': // Set system mode
        systemMode = readIntSerial();
        _queryType = 'X';
        break;
    }
  }
}

void sendSerialTelemetry(){
  switch (_queryType){
    case '=': // Reserved debug command to view any variable from Serial Monitor
      //_queryType = 'X';
      break;
    case 'B': // Send roll and pitch gyro PID values
      //serialPrintPID(ROLL);
      //serialPrintPID(PITCH);
      Serial.println(1300); // TODO: Read from EEPROM
      _queryType = 'X';
      break;
    case 'D': // Send yaw PID values
      //serialPrintPID(YAW);
      serialPrintPID(headingHoldPID);
      Serial.println(0, BIN);
      _queryType = 'X';
      break;
    case 'F': // Send roll and pitch auto level PID values
      serialPrintPID(levelRollPID);
      serialPrintPID(levelPitchPID);
      //serialPrintPID(6); // TODO: LEVELGYROROLL
      serialPrintValueComma(0.00);
      serialPrintValueComma(0.00);
      serialPrintValueComma(0.00);
      //serialPrintPID(7); // TODO: LEVELGYROPITCH
      serialPrintValueComma(0.00);
      serialPrintValueComma(0.00);
      serialPrintValueComma(0.00);
      Serial.println(WINDUP_GUARD_GAIN); // TODO: windup guard
      _queryType = 'X';
      break;
    case 'H': // Send auto level configuration values
      serialPrintValueComma(500.0); // TODO: Read this from EEPROM
      Serial.println(150.0); // TODO: Read this from EEPROM
    
      _queryType = 'X';
      break;
    case 'J': // Altitude Hold
      for(byte i=0; i<9; i++) {
        serialPrintValueComma(0);
      }
      Serial.println('0');
    
      _queryType = 'X';
      break;
    case 'L': // Send data filtering values
      serialPrintValueComma(1.0);
      serialPrintValueComma(accel.getSmoothFactor());
      Serial.println(7.0); // TODO: Read this from EEPROM
      _queryType = 'X';
      break;
    case 'N': // Send transmitter smoothing values
      serialPrintValueComma(0); // TODO? receiver transmit factor
      
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      Serial.println(0.0); // TODO?
      
      _queryType = 'X';
      break;
    case 'P': // Send transmitter calibration data
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      
      serialPrintValueComma(0.0); // TODO?
      serialPrintValueComma(0.0); // TODO?
      
      serialPrintValueComma(0.0); // TODO?
      Serial.println(0.0); // TODO?
      
      _queryType = 'X';
      break;
    case 'Q': // Send sensor data
      serialPrintValueComma(gyro.getRoll());
      serialPrintValueComma(gyro.getPitch());
      serialPrintValueComma(gyro.getYaw());

      serialPrintValueComma(accel.getRoll());
      serialPrintValueComma(accel.getPitch());
      serialPrintValueComma(accel.getYaw());

      serialPrintValueComma(mag.getRaw(XAXIS));
      serialPrintValueComma(mag.getRaw(YAXIS));
      serialPrintValueComma(mag.getRaw(ZAXIS));
      
      serialPrintValueComma(imu.getRoll());
      serialPrintValueComma(imu.getPitch());
      Serial.println(imu.getHeading());
      break;
    case 'R': // *** Spare ***
      break;
    case 'S': // Send all flight data
      serialPrintValueComma(deltaTime);

      // TODO: These are "raw" in the aeroquad version, but that's pretty useless
      serialPrintValueComma(gyro.getRawRoll());
      serialPrintValueComma(gyro.getRawPitch());
      serialPrintValueComma(gyro.getRawYaw());
      serialPrintValueComma(accel.getRawRoll());
      serialPrintValueComma(accel.getRawPitch());
      serialPrintValueComma(accel.getRawYaw());

      serialPrintValueComma(battery.getData()); // Battery monitor
      
      serialPrintValueComma(0); // TODO: Motor axis commands
      serialPrintValueComma(0); // TODO: Motor axis commands
      serialPrintValueComma(0); // TODO: Motor axis commands
      serialPrintValueComma(0); // TODO: Motor axis commands
      
      Serial.print(engines.isArmed(), BIN);
      serialComma();

      serialPrintValueComma(2000); // Always stable mode
      
      serialPrintValueComma(imu.getHeading()); // Heading
      
      serialPrintValueComma(baro.getRawAltitude()); // Alt hold data
      Serial.println(0); // Alt hold on
      break;
    case 'T': // Send processed transmitter values
      serialPrintValueComma(0); // TODO? receiver transmit factor
    
      serialPrintValueComma(0); // TODO? receiver roll
      serialPrintValueComma(0); // TODO? receiver pitch
      serialPrintValueComma(0); // TODO? receiver yaw
      
      serialPrintValueComma(0.0); // TODO? level adjust roll
      serialPrintValueComma(0.0); // TODO? level adjust pitch
      
      serialPrintValueComma(0); // TODO: motor axis roll
      serialPrintValueComma(0); // TODO: motor axis pitch
      Serial.println(0); // TODO: motor axis yaw
      
      break;
    case 'U': // Send smoothed receiver with Transmitter Factor applied values
      serialPrintValueComma(0); // TODO?
      serialPrintValueComma(0); // TODO?
      serialPrintValueComma(0); // TODO?
      serialPrintValueComma(0); // TODO?
      serialPrintValueComma(0); // TODO?
      Serial.println(0); // TODO?
      break;
    case 'V': // Send receiver status
      serialPrintValueComma(receiver.getChannel(ROLL_CHANNEL));
      serialPrintValueComma(receiver.getChannel(PITCH_CHANNEL));
      serialPrintValueComma(receiver.getChannel(YAW_CHANNEL));
      serialPrintValueComma(receiver.getChannel(THROTTLE_CHANNEL));
      serialPrintValueComma(receiver.getChannel(GEAR_CHANNEL));
      Serial.println(receiver.getChannel(AUX_CHANNEL));
      break;
    case 'X': // Stop sending messages
      break;
    case 'Z': // Send heading
      serialPrintValueComma(0); // TODO: receiver.getData(YAW)
      serialPrintValueComma(0.0); // TODO: headingHold
      serialPrintValueComma(0.0); // TODO: setHeading
      Serial.println(0.0); // TODO: relativeHeading
      break;
    case '6': // Report remote commands
      for (byte engine = 0; engine < ENGINE_COUNT-1; engine++){
        serialPrintValueComma(engines.getEngineSpeed(engine)); // TODO: These should be "remote commands"
      }
      Serial.println(engines.getEngineSpeed(ENGINE_COUNT-1)); // TODO: These should be "remote commands"
      break;
    case '!': // Send flight software version
      Serial.println(VERSION, 1);
      _queryType = 'X';
      break;
    case '#': // Send software configuration
      serialPrintValueComma(2); // Emulate AeroQuad_v18
      Serial.print('1'); // X-config
      Serial.println();
      _queryType = 'X';
      break;  
    case 'e': // Send AREF value
      Serial.println(5.0);
      _queryType = 'X';
      break;
    case 'g': // Send magnetometer cal values
      // IGNORED
      _queryType = 'X';
      break;
    case '`': // Send Camera values
      // IGNORED
      break;
    
    // The following modes are my own
    case '&':
      serialPrintValueComma(deltaTime);
      serialPrintValueComma(imu.getRoll());
      serialPrintValueComma(imu.getPitch());
      serialPrintValueComma(imu.getHeading());
      
      serialPrintValueComma(accel.getXAngle());
      serialPrintValueComma(accel.getYAngle());
      serialPrintValueComma(accel.getZAngle());
      
      serialPrintValueComma(gyro.getRoll());
      serialPrintValueComma(gyro.getPitch());
      serialPrintValueComma(gyro.getYaw());
      
      serialPrintValueComma(mag.getRaw(XAXIS));
      serialPrintValueComma(mag.getRaw(YAXIS));
      serialPrintValueComma(mag.getRaw(ZAXIS));
      
      serialPrintValueComma(baro.getRawAltitude());
      serialPrintValueComma(battery.getData());

      serialPrintValueComma(engines.getThrottle());
      
      for (byte engine = 0; engine < ENGINE_COUNT; engine++){
        serialPrintValueComma(engines.getEngineSpeed(engine));
      }
      
      serialPrintValueComma(engines.isArmed());
      Serial.println(systemMode, DEC);
      
      break;
  }
}

// Used to read floating point values from the serial port
float readFloatSerial(){
  #define SERIALFLOATSIZE 10
  byte index = 0;
  byte timeout = 0;
  char data[SERIALFLOATSIZE] = "";

  do {
    if (Serial.available() == 0) {
      delay(10);
      timeout++;
    }
    else {
      data[index] = Serial.read();
      timeout = 0;
      index++;
    }
  }  
  while ((index == 0 || data[index-1] != ';') && (timeout < 5) && (index < sizeof(data)-1));
  data[index] = '\0';
  return atof(data);
}

// Used to read integer values from the serial port
int readIntSerial(){
  #define SERIALINTSIZE 7
  byte index = 0;
  byte timeout = 0;
  char data[SERIALINTSIZE] = "";

  do {
    if (Serial.available() == 0) {
      delay(10);
      timeout++;
    }
    else {
      data[index] = Serial.read();
      timeout = 0;
      index++;
    }
  }  
  while ((index == 0 || data[index-1] != ';') && (timeout < 5) && (index < sizeof(data)-1));
  data[index] = '\0';
  return atoi(data);
}

void serialPrintValueComma(float val){
  Serial.print(val);
  serialComma();
}

void serialPrintValueComma(double val){
  Serial.print(val);
  serialComma();
}

void serialPrintValueComma(char val){
  Serial.print(val);
  serialComma();
}

void serialPrintValueComma(int val){
  Serial.print(val);
  serialComma();
}

void serialPrintValueComma(unsigned long val){
  Serial.print(val);
  serialComma();
}

void serialComma(){
  Serial.print(',');
}

void serialPrintPID(PID pid){
  serialPrintValueComma(pid.getP());
  serialPrintValueComma(pid.getI());
  serialPrintValueComma(pid.getD());
}

void readSerialPID(PID &pid) {
  pid.setP(readFloatSerial());
  pid.setI(readFloatSerial());
  pid.setD(readFloatSerial());
}
