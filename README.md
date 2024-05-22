# DU-UAV Underwater Submersible
Author: John Leseur

*This repository contains all the code and descriptions of code that was used to control our Senior Design project for the University of Denver SRI Lab*

(All code was written inside Arduino IDE)

List of Coded Programs:
- ThrusterTest.ino
- StepperTest.ino
- RX_IntegratedTest.ino
- IMUTurnCorrection.ino
- SimpleTurnAround.ino
- DrainBallast.ino
- NeutralizeMotors.ino

## ThrusterTest.ino
This code controls two servo motors connected to an Arduino or similar microcontroller. Initially, both motors are set to a neutral position. Then, the program cycles through a sequence:

1. Both motors accelerate forward for one second.
2. They return to the neutral position for one second.
3. Both motors accelerate in reverse for one second.
4. They return to the neutral position again for one second.

*This sequence repeats indefinitely.*

## StepperTest.ino
This code operates two stepper motors through an Arduino board and TB6600 stepper motor driver. It establishes pin connections for direction and stepping, sets the total steps for rotation, and continuously executes a loop to control motor movement. The loop increments step counts, performs individual steps for each motor, and halts when the total steps are reached, entering an infinite loop to stop further operation. Overall, it enables continuous rotation of the motors in both directions without relying on external libraries.

It is important to note the polarities of the stepper motors which is seen in the comments. The direction pins are inversed:
* For the Left stepper a signal of a HIGH pulls the stepper in
* For the Right stepper a signal of a LOW pulls the stepper in

## RX_IntegratedTest.ino
This code operates the system with multiple components, including the stepper motors and servo motors. 
It defines pins and variables, sets up interrupt service routines (ISR), and maps receiver input channels to system states. 

1. Two stepper motors' connections are defined, specifying pins for direction and stepping.
2. The code includes functions to handle various states based on receiver input, controlling the motors accordingly (forward, backward, left, right, etc.).
3. There are specific routines for controlling the ballast mechanism using the stepper motors.
4. The updateState() function maps receiver input to system states, prioritizing certain movements.
5. The loop() function continuously updates the system state based on input and executes corresponding actions. Additional functions handle motor control and provide diagnostic information through serial communication.

*Based on system weight distribution, it is encouraged to not use the backwards mapping as the system will tend to dive under the water and lose connection*

## IMUTurnCorrection.ino
This code utilizes the MPU6050 sensor and DMP (Digital Motion Processor) to implement turn correction functionality. It initializes the MPU6050 sensor, calibrates it, and configures the DMP. The main loop continuously monitors sensor data, calculates Euler angles, and adjusts motor outputs accordingly for turn correction.

The Forward() function evaluates the yaw angle and adjusts the thruster motor signals to correct deviations of 10 degrees from the desired heading. This code provides real-time correction based on sensor readings to maintain a straight line.

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



  


