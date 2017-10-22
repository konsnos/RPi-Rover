/*
 *  l298n.c
 *	Handles the L298N Dual H Bridge to control dc motors. Note that this is done without PWM.
 *  To use it initialise the wiring first with the pins associated with it and then drive it.
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "l298n.h"

// Motor 1
int l298n_IN1;
int l298n_IN2;
// Motor 2
int l298n_IN3;
int l298n_IN4;

/// Initialises wiringPi library.
void initialise_l298n(int motor1in1, int motor1in2, int motor2in3, int motor2in4)
{
    if (wiringPiSetup() == -1) {
        exit(EXIT_FAILURE);
    }

    l298n_IN1 = motor1in1;
    l298n_IN2 = motor1in2;
    l298n_IN3 = motor2in3;
    l298n_IN4 = motor2in4;

    pinMode(l298n_IN1, OUTPUT);
    pinMode(l298n_IN2, OUTPUT);
    pinMode(l298n_IN3, OUTPUT);
    pinMode(l298n_IN4, OUTPUT);
}

void forward()
{
    printf("Forward\n");
    digitalWrite(l298n_IN1, LOW);
    digitalWrite(l298n_IN2, HIGH);
    digitalWrite(l298n_IN3, HIGH);
    digitalWrite(l298n_IN4, LOW);
}

void reverse()
{
    printf("Reverse\n");
    digitalWrite(l298n_IN1, HIGH);
    digitalWrite(l298n_IN2, LOW);
    digitalWrite(l298n_IN3, LOW);
    digitalWrite(l298n_IN4, HIGH);
}

void turn_right()
{
    printf("Turning right\n");
    digitalWrite(l298n_IN1, LOW);
    digitalWrite(l298n_IN2, HIGH);
    digitalWrite(l298n_IN3, LOW);
    digitalWrite(l298n_IN4, HIGH);
}

void turn_left()
{
    printf("Turning left\n");
    digitalWrite(l298n_IN1, HIGH);
    digitalWrite(l298n_IN2, LOW);
    digitalWrite(l298n_IN3, HIGH);
    digitalWrite(l298n_IN4, LOW);
}

void stop()
{
    printf("Stopping\n");
    digitalWrite(l298n_IN1, LOW);
    digitalWrite(l298n_IN2, LOW);
    digitalWrite(l298n_IN3, LOW);
    digitalWrite(l298n_IN4, LOW);
}

void cleanUp_l298n()
{
    printf("Cleaning used l298n GPIO pins\n");
    pinMode(l298n_IN1, INPUT);
    pinMode(l298n_IN2, INPUT);
    pinMode(l298n_IN3, INPUT);
    pinMode(l298n_IN4, INPUT);
}

/*int main (int argc, char *argv[])
{
    if (argc != 5) 
    {
        printf("usage: %s <l298n_IN1> <l298n_IN2> <l298n_IN3> <l298n_IN4>\n\nWhere:\n- l298n_IN1 & l298n_IN2 enable Motor1 and set its direction.\n- l298n_IN3 & l298n_IN4 enable Motor2 and set its direction.\nUsing l298n_IN1 %d l298n_IN2 %d l298n_IN3 %d l298n_IN4 %d.\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
        exit(EXIT_FAILURE);
    }

    printf("Raspberry Pi wiringPi L298N Dual H Bridge Motor test program.\n");

    initialise_l298n(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])); // 2 3 21 22

    if (setuid(getuid()) < 0) 
    {
        perror("Dropping privileges failed.\n");
        exit(EXIT_FAILURE);
    }

    int STOP_DELAY = 200;

    delay(50);

    forward();
    delay(1000);
    stop();
    delay(STOP_DELAY);
    reverse();
    delay(1000);
    stop();
    delay(STOP_DELAY);

    turn_left();
    delay(500);
    stop();
    delay(STOP_DELAY);
    turn_right();
    delay(500);
    stop();
    delay(STOP_DELAY);
    
    cleanUp_l298n();
    
    return 0;
}*/