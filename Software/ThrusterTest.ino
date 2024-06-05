/*
#####################################################
# ThrusterTest.ino                                  #
# Created by: John Leseur         Date: 05-22-2024  #
#                                                   #
#                                                   #
# Input(s):   None                                  #
# Output(s):  escL                                  #
#             escR                                  #
#                                                   #
# Description: This code is used for testing the    #
# two thrusters on the system.                      #
#####################################################
*/

#include <Servo.h>

Servo escL; // Create a Servo object
Servo escR; // Create a Servo object

void setup() {
escL.attach(5);
escR.attach(6);

escL.writeMicroseconds(1500);
escR.writeMicroseconds(1500);

delay(3000); // Wait for 3 seconds
}

void loop() {
int throttle = 1500; // Neutral position, motor is not spinning
escL.writeMicroseconds(throttle);
escR.writeMicroseconds(throttle);

// Delay for a certain period of time
delay(1000);

// Accelerate the motor
throttle = 1600;
escL.writeMicroseconds(throttle);
escR.writeMicroseconds(throttle);

// Delay for a certain period of time
delay(1000);

throttle = 1500;
escL.writeMicroseconds(throttle);
escR.writeMicroseconds(throttle);

// Delay for a certain period of time
delay(1000);

// Reverse the motor
throttle = 1400;
escL.writeMicroseconds(throttle);
escR.writeMicroseconds(throttle);

// Delay for a certain period of time
delay(1000);

// Neutral position, motor is not spinning
throttle = 1500;
escL.writeMicroseconds(throttle);
escR.writeMicroseconds(throttle);

// Delay for a certain period of time
delay(1000);
}