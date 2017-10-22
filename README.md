# README #

Drives a rover avoiding obstacles with an L298N Dual H Bridge controller for the dc motors and HC-SR04 Ultrasonic Sensor to sense obstacles.

### How do I get set up? ###

* You'll need WiringPi library. For instructions check this: http://wiringpi.com/download-and-install/

### How to run ###

The program has been tested in Raspbian and Raspberry Pi Zero. The program assumes some pin configuration which can be found and changed in the rover.c file in the initialisation function. To run it, type make in the folder to use the makefile and compile it. Then run ./rover to run the program.