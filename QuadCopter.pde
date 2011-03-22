// QUADCOPTER!!!!
// by Myles Grant <myles@mylesgrant.com>
// See also: https://github.com/grantmd/QuadCopter

#include <Servo.h> 

// Engines
const byte ENGINE_COUNT = 4;
Servo left_front;  // pin 3
Servo right_front; // pin 9
Servo left_rear;   // pin 10
Servo right_rear;  // pin 11
Servo engines[ENGINE_COUNT] = {left_front, right_front, left_rear, right_rear};
int engine_speeds[ENGINE_COUNT];

const int MOTOR_ARM = 9; // Value for my ESCs. Yours is different!
const int MIN_MOTOR_SPEED = 50;
const int MAX_MOTOR_SPEED = 150;

// Activity
byte red_led = 12; // System status
byte green_led = 13; // Motor activity

// Status
boolean is_armed = false; // System has been armed
int system_mode = 0; // 0: Unknown, 1: Auto, 2: Manual

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
  Serial.println("Choose mode (1: Auto, 2: Manual)");
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
  Serial.print("Setting speed to: ");
  Serial.println(speed);
  for (byte engine = 0; engine < ENGINE_COUNT; engine++){
    set_engine_speed(engine, speed);
  }
}

void loop(){
  if (system_mode == 1){
    if (!is_armed) arm();
    
    // Accelerate to max speed in 100ms increments
    Serial.println("Accelerating");
     digitalWrite(green_led, HIGH);
    for(pos = MIN_MOTOR_SPEED; pos <= MAX_MOTOR_SPEED; pos += 1){
      set_all_speed(pos);
      delay(1000);
    }
    
    // Hold at max speed
    Serial.println("Holding at max speed");
    digitalWrite(green_led, LOW);
    delay(5000);
    
    // Decelerate to stop in 100ms increments
    Serial.println("Decelerating");
    digitalWrite(green_led, HIGH);
    for(pos = MAX_MOTOR_SPEED; pos >= MIN_MOTOR_SPEED; pos -= 1){
      set_all_speed(pos);
      delay(1000);
    }
    
    // Hold at stop
    Serial.println("Holding at stop");
    digitalWrite(green_led, LOW);
    delay(5000);
  }
  else if (Serial.available() > 0){
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
      set_all_speed(input_int);
    }
    else{
      system_mode = input_int;
    }
    
    // Prompt for next speed
    if (system_mode == 2){
      Serial.println("Enter speed (0-180):");
    }
  }
} 
