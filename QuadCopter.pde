// QUADCOPTER!!!!
// by Myles Grant <myles@mylesgrant.com>
// See also: https://github.com/grantmd/QuadCopter

#include <Servo.h> 
 
Servo left_front;  // pin 3
Servo right_front; // pin 9
Servo left_rear;   // pin 10
Servo right_rear;  // pin 11

int red_led = 12; // System status
int green_led = 13; // Motor activity

const int MOTOR_ARM = 10; // Value for my ESCs. Yours is different!
boolean is_armed = false; // System has been armed

int pos = 0;
 
void setup(){
  Serial.begin(9600);
  
  // Activity LEDs
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  
  digitalWrite(red_led, HIGH); // Indicate system is not ready
  
  // Setup motors
  left_front.attach(3);
  right_front.attach(9);
  left_rear.attach(10);
  right_rear.attach(11);
  
  Serial.println("Setup complete");
  arm();
}

void arm(){
  Serial.println("Arming");
  
  // arm the speed controllers
  left_front.write(MOTOR_ARM);
  right_front.write(MOTOR_ARM);
  left_rear.write(MOTOR_ARM);
  right_rear.write(MOTOR_ARM);
  delay(5000); // wait for arming/getting out of the way
  is_armed = true;
  
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, HIGH); // System is ready
  Serial.println("Armed");
}

void all_stop(){
  left_front.write(MOTOR_ARM);
  right_front.write(MOTOR_ARM);
  left_rear.write(MOTOR_ARM);
  right_rear.write(MOTOR_ARM);
  
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, LOW); // System is NOT ready!
  Serial.println("All stop");
}

void loop(){
  // Accelerate to max speed in 100ms increments
  Serial.println("Accelerating");
  for(pos = MOTOR_ARM; pos < 180; pos += 1){
    digitalWrite(green_led, LOW);
    left_front.write(pos);
    right_front.write(pos);
    left_rear.write(pos);
    left_rear.write(pos);
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
    left_front.write(pos);
    right_front.write(pos);
    left_rear.write(pos);
    right_rear.write(pos);
    delay(50);
    digitalWrite(green_led, HIGH);
    delay(50);
  }
  
  // Hold at stop
  Serial.println("Holding at stop");
  delay(5000);
} 
