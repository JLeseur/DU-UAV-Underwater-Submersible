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
* For the left stepper a signal of a HIGH pulls the stepper in
* For the Right stepper a signal of a LOW pulls the stepper in

  


