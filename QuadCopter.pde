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

// Speeds are in microseconds, not degrees, for greater control
const int MIN_MOTOR_SPEED = 1050; // Value for my ESCs and motors. Yours is different!
const int MAX_MOTOR_SPEED = 2000; // Value for my ESCs and motors. Yours is different!

// Activity
byte red_led = 12; // System status
byte green_led = 13; // Motor activity

// Status
int system_mode = 0; // 0: Unknown, 1: Auto, 2: Manual

int pos = 0;
 
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

void all_stop(){
  set_all_speed(MIN_MOTOR_SPEED);
  
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, LOW); // System is NOT ready!
  Serial.println("All stop");
}

void set_engine_speed(int engine, int speed){
  engines[engine].writeMicroseconds(speed);
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
    
    // Accelerate to max speed
    Serial.println("Accelerating");
    digitalWrite(green_led, HIGH);
    for(pos = MIN_MOTOR_SPEED; pos <= MAX_MOTOR_SPEED; pos += 10){
      set_all_speed(pos);
      delay(500);
    }
    
    // Hold at max speed
    Serial.println("Holding at max speed");
    digitalWrite(green_led, LOW);
    delay(5000);
    
    // Decelerate to stop
    Serial.println("Decelerating");
    digitalWrite(green_led, HIGH);
    for(pos = MAX_MOTOR_SPEED; pos >= MIN_MOTOR_SPEED; pos -= 10){
      set_all_speed(pos);
      delay(500);
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
      Serial.println("Enter speed (1000-2000):");
    }
  }
} 
