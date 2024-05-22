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

##ThrusterTest.ino
This code controls two servo motors connected to an Arduino or similar microcontroller. Initially, both motors are set to a neutral position. Then, the program cycles through a sequence:

Both motors accelerate for one second.
They return to the neutral position for one second.
Both motors reverse direction for one second.
They return to the neutral position again for one second.
This sequence repeats indefinitely.
  


