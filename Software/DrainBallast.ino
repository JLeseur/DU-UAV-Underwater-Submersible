/*
#####################################################
# DrainBallast.ino                                  #
# Created by: John Leseur         Date: 05-29-2024  #
#                                                   #
#                                                   #
# Input(s):   None                                  #
# Output(s):  dirPinL                               #
#             dirPinR                               #
#             stepPinL                              #
#             stepPinR                              #
#                                                   #
# Description: This code is used to drain the       #
# ballast tanks after use. If for some reason water # 
# managed to get into the ballast tanks, run this   #
# code to push the ballast out, the water will drain#
# then the ballast will pull themself back in.      #
#####################################################
*/

// Define stepper motor connections:
#define dirPinL 3
#define stepPinL 4
#define dirPinR 12
#define stepPinR 13

// Set the total number of steps to rotate before stopping
const int totalSteps = 3000;

// Set Step Counter variable
int stepCount = 0;

// Declare the states
enum State {
  DRAIN,
  PULLIN,
  IDLE
};

// Initialize the state
State currentState = DRAIN;

void setup() {
  // Declare pins as output:
  pinMode(stepPinL, OUTPUT);
  pinMode(dirPinL, OUTPUT);
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinR, OUTPUT);

  digitalWrite(stepPinL, LOW);
  digitalWrite(stepPinR, LOW);
}

void loop() {
  // Execute actions based on the current state
  switch (currentState) {
    case DRAIN:
      Serial.print("Draining Ballast...\n");
      ballastDown();
      delay(500);
      break;

    case PULLIN:
      Serial.print("Pulling Ballast Back In...\n");
      ballastUp();
      break;

    case IDLE:
      Serial.print("Idle...\n");
      Idle();
      break;
  }
}

void ballastUp() {
  // Set the stepper spinning direction
  digitalWrite(dirPinL, HIGH);  // High pulls plunger in for LStep
  digitalWrite(dirPinR, LOW);   // LOW pulls plunger in for RStep

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
        currrentState = IDLE;
        break;
      }
    }
  }
}


void ballastDown() {
  // Set the stepper spinning direction
  digitalWrite(dirPinL, LOW);   // LOW pushes plunger out for LStep
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
        currentState = PULLIN;        
        break;
      }
    }
  }
}

void Idle() {
      digitalWrite(stepPinL, LOW);
      digitalWrite(stepPinR, LOW);
}