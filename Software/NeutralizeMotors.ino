/*
#####################################################
# NeutralizeMotors.ino                              #
# Created by: John Leseur         Date: 05-29-2024  #
#                                                   #
#                                                   #
# Input(s):   None                                  #
# Output(s):  escL                                  #
#             escR                                  #
#             stepPinL                              #
#             stepPinR                              #
#             dirPinL                               #
#             dirPinR                               #
#                                                   #
# Description: This code is used to neutralize      #
# all the motors. This is good to do after running  #
# autonomous code as if the autonomous code is left #
# uploaded, on the next startup of the system the   # 
# autonomous code will run. So upload this after    #
# running autonomous code to set all the motors to  #
# a neutral point.                                  #
#####################################################
*/

#include <Servo.h>

// Define stepper motor connections:
#define dirPinL 3
#define stepPinL 4
#define dirPinR 12
#define stepPinR 13

Servo escL;  // Create a Servo object for thruster Left
Servo escR;  // Create a Servo object for thruster Right

void setup() {
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
  digitalWrite(stepPinL, LOW);   // Initialize steppers off
  digitalWrite(stepPinR, LOW);   // Initialize steppers off
}

void loop() {
  escL.writeMicroseconds(1500);  // Initialize the motor to a neutral position
  escR.writeMicroseconds(1500);  // Initialize the motor to a neutral position
  digitalWrite(stepPinL, LOW);   // Initialize steppers off
  digitalWrite(stepPinR, LOW);   // Initialize steppers off
}
