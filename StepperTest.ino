/*
#####################################################
# StepperTest.ino                                   #
# Created by: John Leseur         Date: 05-22-2024  #
#                                                   #
#                                                   #
# Input(s):   None                                  #
# Output(s):  dirPinL                               #
#             dirPinR                               #
#             stepPinL                              #
#             stepPinR                              #
#                                                   #
# Description: This code is used for testing the    #
# two stepper motors on the system.                 #
#####################################################
*/

// Define stepper motor connections:
#define dirPinL 2
#define stepPinL 3
#define dirPinR 12
#define stepPinR 13

// Set the total number of steps to rotate before stopping
const int totalSteps = 1000;

void setup() {
  // Declare pins as output:
  pinMode(stepPinL, OUTPUT);
  pinMode(dirPinL, OUTPUT);
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinR, OUTPUT);

  digitalWrite(stepPinL, LOW);
  digitalWrite(stepPinR, LOW);

  // Set the spinning direction CW/CCW:
  digitalWrite(dirPinL, LOW); // High pulls plunger in for LStep
  digitalWrite(dirPinR, HIGH); // Low pulls plunger in for RStep
}

void loop() {
  // Initialize the step count
  static int stepCount = 0;

  // Check if the total number of steps has been reached
  if (stepCount >= totalSteps) {
    // Stop the stepper motor
    while (true) {
      // Infinite loop to keep the program from running further
    }
  }

  // Perform one step:
  digitalWrite(stepPinL, HIGH);
  digitalWrite(stepPinR, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPinL, LOW);
  digitalWrite(stepPinR, LOW);
  delayMicroseconds(500);

  // Increment the step count
  stepCount++;
}