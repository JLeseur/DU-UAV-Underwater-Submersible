# DU-UAV Underwater Submersible
Author: John H. Leseur

Last Updated: *22 May 2024*

*This repository contains all the code and descriptions of code that was used to control our Senior Design project for the University of Denver Unmanned Systems Research Institute Lab*

(All code was written inside Arduino IDE)

*For the IMUTurnCorrection and SimpleTurnAround code I used Jeff Rowbergs library for the MPU6050. This will need to be downloaded for the code to work. The library can be found here under /Arduino/MPU6050: https://github.com/jrowberg/i2cdevlib*

List of Coded Programs:
- ThrusterTest.ino
- StepperTest.ino
- RX_IntegratedTest.ino
- IMUTurnCorrection.ino
- SimpleTurnAround.ino
- DrainBallast.ino
- NeutralizeMotors.ino

## ThrusterTest.ino
This code controls two thruster motors connected to an Arduino UNO. Initially, both motors are set to a neutral position. Then, the program cycles through a sequence:

1. Both motors accelerate forward for one second.
2. They return to the neutral position for one second.
3. Both motors accelerate in reverse for one second.
4. They return to the neutral position again for one second.

*This sequence repeats indefinitely.*

## StepperTest.ino
This code operates two stepper motors through an Arduino UNO and TB6600 stepper motor driver. It establishes pin connections for direction and stepping, sets the total steps for rotation, and continuously executes a loop to control motor movement. The loop increments step counts, performs individual steps for each motor, and halts when the total steps are reached, entering an infinite loop to stop further operation. Overall, it enables continuous rotation of the motors in both directions without relying on external libraries.

It is important to note the polarities of the stepper motors which is seen in the comments. The direction pins are inversed:
* For the Left stepper a signal of a HIGH pulls the stepper in
* For the Right stepper a signal of a LOW pulls the stepper in

## RX_IntegratedTest.ino
This code operates the system with multiple components, including the stepper motors and thruster motors. 
It defines pins and variables, sets up interrupt service routines (ISR), and maps receiver input channels to system states. 
The RX controller used for the system is the Spektrum DX6i Controller.

1. Two stepper motors' connections are defined, specifying pins for direction and stepping.
2. The code includes functions to handle various states based on receiver input, controlling the thruster motors accordingly (forward, backward, left, right, etc.).
3. There are specific routines for controlling the ballast mechanism using the stepper motors.
4. The updateState() function maps receiver input to system states, prioritizing certain movements.
5. The loop() function continuously updates the system state based on input and executes corresponding actions. Additional functions handle motor control and provide diagnostic information through serial communication.

To operate the system the controls are as follows:
- Right Joystick: Controls movement of the systems. The system is capable of Forward, Backard, Left, or Right movement. This is en Exclusive OR, so only one direction can be used at a time.
- Left Joystick: Controls the Submersion protocol. The submersion protocol is a timed protocol when the system will submerge and wait for a specified time till it initiates it's Resurfacing protocol. To enter this protocol, simply push the left joystick all the way up, then pull the joystick back down to it's bottom position. (This is necessary as if the left joystick is left in the upper position, the system will continuously loop through the submersion protocol.)

*Note: Based on system weight distribution, it is highly encouraged to not use the backwards mapping as the system will tend to dive under the surface of the water and lose connection*

## IMUTurnCorrection.ino
This code utilizes the MPU6050 sensor and DMP (Digital Motion Processor) to implement turn correction functionality. It initializes the MPU6050 sensor, calibrates it, and configures the DMP. The main loop continuously monitors sensor data, calculates Euler angles, and adjusts motor outputs accordingly for turn correction.

The Forward() function evaluates the yaw angle and adjusts the thruster motor signals to correct deviations of 10 degrees from the desired heading. This code provides real-time correction based on sensor readings to maintain driving in a straight line.

*This sequence repeats indefinitely. So there is no termination, this is used as a bench test of the IMU and Thrusters*

## SimpleTurnAround.ino
1. Initialization:

Initializes the MPU6050 sensor and sets up communication via I2C.
Sets up the Serial communication for debugging purposes.
Initializes servo motors for controlling the system's movement.

2. Main Program Loop:

Waits for interrupts from the MPU6050 or for new data packets.
Processes MPU data when available.

3. Main Program Functions:

  * StartUp:

Initializes the system, ensuring both motors are set to neutral.
Transitions to the FORWARD state after a set time.

  * Forward:

Moves the system forward for a specified distance at a constant velocity.
Adjusts motor speeds based on orientation data from the MPU6050.
Transitions to the STOP state when the set distance is reached.

  * Return:

Moves the system back to its starting position using orientation data.
Similar to the Forward state but in the opposite direction.
Transitions to the IDLE state upon reaching the starting position.

  * Turn:

Executes a turn around maneuver by adjusting motor speeds.
Transitions to the RETURN state after completing the turn.

  * Stop:

Brings the system to a stop after it has moved forward.
Transitions to the TURNING state to initiate the turn around.

  * Idle:

Sets both motors to a neutral position, halting the system's movement.

* Helper Functions:
  - dmpDataReady: Interrupt service routine triggered when MPU6050 data is ready.
  - getPosition: Retrieves orientation data (yaw, pitch, roll) from the MPU6050.

## DrainBallast.ino
The code controls two stepper motors connected to the system's ballast tanks. 

It defines three main states: "DRAIN," "PULLIN," and "IDLE."

* In the "DRAIN" state, the system activates the stepper motors to push the ballast out, effectively draining water from the tanks.
* Once the draining process is complete, the system transitions to the "PULLIN" state, where the motors then pull the ballast in until it reaches its original position.
* After completing the "PULLIN" operation, the system goes into an "IDLE" state.

*Note: This code usually doesn't need to be used and most troubleshooting or ballast draining should be done by adjusting the step count on the StepperTest.ino for more fine tuned control of the ballast*

## NeutralizeMotors.ino
This code is used to neutralize all motors on the system. The pins for two stepper motors are defined as dirPinL, stepPinL, dirPinR, and stepPinR. Both thrusters are attached to pins 5 and 6.

In the setup function, both thrusters are initialized to a neutral position of 1500 microseconds, and the stepper motors are turned off by setting their pins to LOW.

The loop function continuously repeats, ensuring that both thrusters are maintained in their neutral positions and both stepper motors are turned off by setting their pins to LOW. This loop ensures that all motors remain inactive, effectively neutralizing any movement or operation.

*Note: This code is only really necessary for after performing autonomous control (i.e. SimpleTurnAround.ino, IMUTurnCorrection). Because after shutdown, if the system is left with the autonomous control uploaded it will start up and perform the autonomous control on next boot up.*




  


