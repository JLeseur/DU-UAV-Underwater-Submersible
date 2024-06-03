/*
#####################################################
# RX_IntegratedTest.ino                             #
# Created by: John Leseur         Date: 05-29-2024  #
#                                                   #
#                                                   #
# Input(s):   PCINT0                                #
#             PCINT1                                #
#             PCINT2                                #
#             PCINT3                                #
# Output(s):  escL                                  #
#             escR                                  #
#             stepPinL                              #
#             stepPinR                              #
#             dirPinL                               #
#             dirPinR                               #
#                                                   #
# Description: This code is used for testing the    #
# entire system using the Spektrum DX6i Controller  # 
# The controller allows for control of the two      #
# thrusters and of the ballast tank steppers        #
# Simple control goes as follows: Right joystick    #
# controls the thrusters. You can go Forward, Left, #
# Right, or Backwards. This is an exclusive OR. So, #
# You can only in one direction at a time. The left #
# joystick controls the ballast tanks. Push the     #
# joystick all the way up to initiate the           #
# submersion protocol, then pull the joystick back  #
# down to the bottom position.                      #
#####################################################
*/

#include <Servo.h>

//Declaring Variables
byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
int receiver_input_channel_1, receiver_input_channel_2, receiver_input_channel_3, receiver_input_channel_4;
unsigned long timer_1, timer_2, timer_3, timer_4;

// Define stepper motor connections:
#define dirPinL 3
#define stepPinL 4
#define dirPinR 12
#define stepPinR 13

// Set the total number of steps to rotate before stopping
// (Starting position is when ballast plunger is flush with the pipe)
const int totalSteps = 30000; // Max stepper range is around 32000 steps from starting position (If starting position changes, this will need to be adjusted.)

// Set Step Counter variable
int stepCount = 0;

Servo escL;  // Create a Servo object for thruster Left
Servo escR;  // Create a Servo object for thruster Right

// Declare the states
enum State {
  IDLE,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  RESUB
};

// Initialize the state
State currentState = IDLE;

//Setup routine
void setup() {
  //Arduino (Atmega) pins default to inputs, so they don't need to be explicitly declared as inputs
  PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);  // set PCINT0 (digital input 8) to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT1);  // set PCINT1 (digital input 9)to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT2);  // set PCINT2 (digital input 10)to trigger an interrupt on state change
  PCMSK0 |= (1 << PCINT3);  // set PCINT3 (digital input 11)to trigger an interrupt on state change
  Serial.begin(9600);

  escR.attach(6);  // Attach the ESC to pin 6 on the Arduino
  escL.attach(5);  // Attach the ESC to pin 5 on the Arduino

  // Declare stepper pins as output:
  pinMode(stepPinL, OUTPUT);
  pinMode(dirPinL, OUTPUT);
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinR, OUTPUT);

  escL.writeMicroseconds(1500);  // Initialize the motor to a neutral position
  escR.writeMicroseconds(1500);  // Initialize the motor to a neutral position
  digitalWrite(stepPinL, LOW); // Initialize steppers off
  digitalWrite(stepPinR, LOW); // Initialize steppers off

  delay(2000);  // Wait for 2 seconds
}

void loop() {
  delay(250);
  //print_signals()   

  updateState(); // Update the state depending on controller input

  // Execute actions based on the current state
  switch (currentState) {
    case FORWARD:
      Serial.print("Forward\n");
      Forward();
      break;

    case BACKWARD:
      Serial.print("Backward\n");
      Backward();
      break;

    case LEFT:
      Serial.print("Left\n");
      Left();
      break;

    case RIGHT:
      Serial.print("Right\n");
      Right();
      break;
    
    case RESUB:
      Serial.print("Performing ReSub protocol...\n");
      ballastUp(); // Pull plunger in
      digitalWrite(stepPinL, LOW); // turn steppers off
      digitalWrite(stepPinR, LOW); // turn steppers off

      delay(5000); // Specify the desired time for system to sit underwater before resurfacing

      ballastDown(); // Pushes plunger out
      digitalWrite(stepPinL, LOW); // turn steppers off
      digitalWrite(stepPinR, LOW); // turn steppers off
      break;

    case IDLE:
      Serial.print("Idle\n");
      Stop();
      break;
  }
}

void updateState() {
  // Determine the state based on receiver input
  int left_right = map(receiver_input_channel_2, 1096, 1896, 1000, 2000);
  int forward_backward = map(receiver_input_channel_3, 1088, 1904, 1000, 2000);

  // Mapping ranges for forward/backward and left/right movement
  int forward_backward_low = 1400;   // Adjust as needed
  int forward_backward_high = 1600;  // Adjust as needed
  int left_right_low = 1400;         // Adjust as needed
  int left_right_high = 1600;        // Adjust as needed
  int ballast_low = 1300;
  int ballast_high = 1800;

  // Read the mapped input value from the remote control
  int ballast_up_down = map(receiver_input_channel_1, 1080, 1916, 1000, 2000);

  // Prioritize forward/backward movement coming from the user inputs
  if (forward_backward > forward_backward_high) {
    currentState = FORWARD;
  } else if (forward_backward < forward_backward_low) {
    currentState = BACKWARD;
  } else if (left_right > left_right_high) {
    currentState = LEFT;
  } else if (left_right < left_right_low) {
    currentState = RIGHT;
  } else if (ballast_up_down > ballast_high) {
    currentState = RESUB;
  } else {
    digitalWrite(stepPinL, LOW); // Keep Steppers off
    digitalWrite(stepPinR, LOW); // Keep Steppers off
    currentState = IDLE;
  }
}

void Forward() {
  // Thruster control logic for backward movement
  int throttle = map(receiver_input_channel_3, 1088, 1904, 1000, 2000);
  escL.writeMicroseconds(throttle);
  escR.writeMicroseconds(throttle);
}

void Backward() {
  // Thruster control logic for forward movement
  int throttle = map(receiver_input_channel_3, 1088, 1904, 1000, 2000);
  escL.writeMicroseconds(throttle);
  escR.writeMicroseconds(throttle);
}

void Left() {
  // Thruster control logic for left turn
  int throttle = map(receiver_input_channel_2, 1096, 1896, 1000, 2000);
  escR.writeMicroseconds(throttle);
}

void Right() {
  // Thruster control logic for right turn
  int throttle = map(receiver_input_channel_2, 1896, 1096, 1000, 2000);
  escL.writeMicroseconds(throttle);
}

void Stop() {
  escL.writeMicroseconds(1500);
  escR.writeMicroseconds(1500);
}

void ballastUp() {
  // Read the mapped input value from the remote control
  int ballast_up_down = map(receiver_input_channel_1, 1080, 1916, 0, totalSteps);

  // Set the stepper spinning direction
  digitalWrite(dirPinL, HIGH);  // High pulls plunger in for LStep
  digitalWrite(dirPinR, LOW); // LOW pulls plunger in for RStep

  // Perform steps only if there are remaining steps to move
  if (stepCount < totalSteps) {
    // Perform steps
    for (int i = 0; i < totalSteps; i++) {
  
      digitalWrite(stepPinL, HIGH);
      digitalWrite(stepPinR, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPinL, LOW);
      digitalWrite(stepPinR, LOW);
      delayMicroseconds(500);

      // Increment the step count
      stepCount++;

      // Break the loop if the total steps are reached
      if (stepCount >= totalSteps) {
        break;
      }
    }
  }
}

void ballastDown() {
  // Read the mapped input value from the remote control
  int ballast_up_down = map(receiver_input_channel_1, 1080, 1916, 0, totalSteps);

  // Set the stepper spinning direction
  digitalWrite(dirPinL, LOW);  // LOW pushes plunger out for LStep
  digitalWrite(dirPinR, HIGH);  // HIGH pushes plunger out on RStep

  // Perform steps only if there are remaining steps to move
  if (stepCount > 0) {
    // Perform steps
    for (int i = 0; i < totalSteps; i++) {
            
      digitalWrite(stepPinL, HIGH);
      digitalWrite(stepPinR, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPinL, LOW);
      digitalWrite(stepPinR, LOW);
      delayMicroseconds(500);

      // Decrement the step count
      stepCount--;

      // Break the loop if the total steps are reached
      if (stepCount <= 0) {
        break;
      }
    }
  }
}

//This routine is called every time input 8(Ballast Up/Down), 9(Left/Right), 10(Forward/Backward) or 11(N/A) changed state
ISR(PCINT0_vect) {
  //Channel 1=========================================
  if (last_channel_1 == 0 && PINB & B00000001) {            //Input 8 changed from 0 to 1
    last_channel_1 = 1;                                     //Remember current input state
    timer_1 = micros();                                     //Set timer_1 to micros()
  } else if (last_channel_1 == 1 && !(PINB & B00000001)) {  //Input 8 changed from 1 to 0
    last_channel_1 = 0;                                     //Remember current input state
    receiver_input_channel_1 = micros() - timer_1;          //Channel 1 is micros() - timer_1
  }
  //Channel 2=========================================
  if (last_channel_2 == 0 && PINB & B00000010) {            //Input 9 changed from 0 to 1
    last_channel_2 = 1;                                     //Remember current input state
    timer_2 = micros();                                     //Set timer_2 to micros()
  } else if (last_channel_2 == 1 && !(PINB & B00000010)) {  //Input 9 changed from 1 to 0
    last_channel_2 = 0;                                     //Remember current input state
    receiver_input_channel_2 = micros() - timer_2;          //Channel 2 is micros() - timer_2
  }
  //Channel 3=========================================
  if (last_channel_3 == 0 && PINB & B00000100) {            //Input 10 changed from 0 to 1
    last_channel_3 = 1;                                     //Remember current input state
    timer_3 = micros();                                     //Set timer_3 to micros()
  } else if (last_channel_3 == 1 && !(PINB & B00000100)) {  //Input 10 changed from 1 to 0
    last_channel_3 = 0;                                     //Remember current input state
    receiver_input_channel_3 = micros() - timer_3;          //Channel 3 is micros() - timer_3
  }
  //Channel 4=========================================
  if (last_channel_4 == 0 && PINB & B00001000) {            //Input 11 changed from 0 to 1
    last_channel_4 = 1;                                     //Remember current input state
    timer_4 = micros();                                     //Set timer_4 to micros()
  } else if (last_channel_4 == 1 && !(PINB & B00001000)) {  //Input 11 changed from 1 to 0
    last_channel_4 = 0;                                     //Remember current input state
    receiver_input_channel_4 = micros() - timer_4;          //Channel 4 is micros() - timer_4
  }
}
//Subroutine for displaying the receiver signals
void print_signals() {
  Serial.print("Ballast Up/Down");
  if (receiver_input_channel_1 - 1480 < 0) Serial.print("<<<");
  else if (receiver_input_channel_1 - 1520 > 0) Serial.print(">>>");
  else Serial.print("-+-");
  Serial.print(receiver_input_channel_1);

  Serial.print("  Left/Right:");
  if (receiver_input_channel_2 - 1480 < 0) Serial.print("^^^");
  else if (receiver_input_channel_2 - 1520 > 0) Serial.print("vvv");
  else Serial.print("-+-");
  Serial.print(receiver_input_channel_2);

  Serial.print("  Forward/Backward:");
  if (receiver_input_channel_3 - 1480 < 0) Serial.print("vvv");
  else if (receiver_input_channel_3 - 1520 > 0) Serial.print("^^^");
  else Serial.print("-+-");
  Serial.print(receiver_input_channel_3);

  Serial.print("  N/A");
  if (receiver_input_channel_4 - 1480 < 0) Serial.print("<<<");
  else if (receiver_input_channel_4 - 1520 > 0) Serial.print(">>>");
  else Serial.print("-+-");
  Serial.println(receiver_input_channel_4);
}
