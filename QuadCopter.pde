// QUADCOPTER!!!!
// by Myles Grant <myles@mylesgrant.com>
// See also: https://github.com/grantmd/QuadCopter

#include <Servo.h> 

// Engines
const byte engine_count = 4;
Servo left_front;  // pin 3
Servo right_front; // pin 9
Servo left_rear;   // pin 10
Servo right_rear;  // pin 11
Servo engines[engine_count] = {left_front, right_front, left_rear, right_rear};
byte engine_speeds[engine_count];
const byte MOTOR_ARM = 9; // Value for my ESCs. Yours is different!

// Activity
byte red_led = 12; // System status
byte green_led = 13; // Motor activity

// Status
boolean is_armed = false; // System has been armed
boolean is_ready = false; // The operator (that's you!) indicates the system is ready
int power_up_time;

byte pos = 0;
 
void setup(){
  Serial.begin(9600);
  
  // Activity LEDs
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  
  digitalWrite(red_led, HIGH); // Indicate system is not ready
  
  // Setup motors
  engines[0].attach(3);
  engines[1].attach(9);
  engines[2].attach(10);
  engines[3].attach(11);
  
  all_stop(); // Just in case we were reset while engines were running
  
  Serial.println("Setup complete");
  Serial.println("Enter 'Y' to continue...");
}

void arm(){
  Serial.println("Arming");
  
  // arm the speed controllers
  set_all_speed(MOTOR_ARM);
  delay(5000); // wait for arming/getting out of the way
  is_armed = true;
  
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, HIGH); // System is ready
  Serial.println("Armed");
}

void all_stop(){
  set_all_speed(0);
  
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, LOW); // System is NOT ready!
  Serial.println("All stop");
}

void set_engine_speed(int engine, int speed){
  engines[engine].write(speed);
  engine_speeds[engine] = speed;
}

void set_all_speed(int speed){
  for (byte engine=0; engine<engine_count; engine++){
    set_engine_speed(engine, speed);
  }
}

void loop(){
  if (is_ready){
    if (!is_armed) arm();
    
    // Accelerate to max speed in 100ms increments
    Serial.println("Accelerating");
    for(pos = MOTOR_ARM; pos < 180; pos += 1){
      digitalWrite(green_led, LOW);
      set_all_speed(pos);
      delay(50);
      digitalWrite(green_led, HIGH);
      delay(50);
    }
    
    // Hold at max speed
    Serial.println("Holding at max speed");
    delay(5000);
    
    // Decelerate to stop in 100ms increments
    Serial.println("Decelerating");
    for(pos = 180; pos>=MOTOR_ARM+1; pos-=1){
      digitalWrite(green_led, LOW);
      set_all_speed(pos);
      delay(50);
      digitalWrite(green_led, HIGH);
      delay(50);
    }
    
    // Hold at stop
    Serial.println("Holding at stop");
    delay(5000);
  }
  else if (Serial.available() > 0){
    if (Serial.read() == 'Y') is_ready = true;
  }
} 
