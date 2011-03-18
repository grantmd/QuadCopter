// QUADCOPTER!!!!
// by Myles Grant <myles@mylesgrant.com>

#include <Servo.h> 
 
Servo leftFront;
Servo rightFront;
Servo leftRear;
Servo rightRear;
 
void setup() 
{ 
  leftFront.attach(3);
  rightFront.attach(9);
  leftRear.attach(10);
  rightRear.attach(11);
} 
 
void loop() 
{ 
  
} 
