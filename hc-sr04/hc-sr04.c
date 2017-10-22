/*
 *  hc-sr04.c
 *	Handles the HC-SR04 ultrasonic distance sensor.
 *  To use it initialise the wiring first with the function initialiseWiring with the trigger and echo pins and then use the function calculateDistance to get the distance in centimeters.
 */

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

#include "hc-sr04.h"

int hc_sr04_TRIG;
int hc_sr04_ECHO;

/// Initialises wiringPi library.
void initialise_hc_sr04(int newTrig, int newEcho)
{
    if (wiringPiSetup() == -1) {
        exit(EXIT_FAILURE);
    }

    hc_sr04_TRIG = newTrig;
    hc_sr04_ECHO = newEcho;

    pinMode(hc_sr04_TRIG, OUTPUT);
    pinMode(hc_sr04_ECHO, INPUT);
}

/// Returns the distance in centimeters
float calculateDistance()
{
    long ping      = 0;
    long pong      = 0;
    float distance = 0;
    long timeout   = micros() + 500000; // 0.5 sec ~ 171 m
    
    // Ensure trigger is low.
    digitalWrite(hc_sr04_TRIG, LOW);
    delay(50);
    
    // Trigger the ping.
    digitalWrite(hc_sr04_TRIG, HIGH);
    delayMicroseconds(10); 
    digitalWrite(hc_sr04_TRIG, LOW);
    
    // Wait for ping response, or timeout.
    while (digitalRead(hc_sr04_ECHO) == LOW && micros() < timeout) {
    }
    
    // Cancel on timeout.
    if (micros() > timeout) {
        printf("Out of range.\n");
        return 0;
    }
    
    ping = micros();
    
    // Wait for pong response, or timeout.
    while (digitalRead(hc_sr04_ECHO) == HIGH && micros() < timeout) {
    }
    
    // Cancel on timeout.
    if (micros() > timeout) {
        printf("Out of range.\n");
        return 0;
    }
    
    pong = micros();
    
    // Convert ping duration to distance.
    distance = (float) (pong - ping) * 0.017150;
    
    //printf("Distance: %.2f cm.\n", distance);
    
    return distance;
}

/// This is to ensure that a missed distance calculation will be ignored.
float getMiddleOfThree()
{
    float a = calculateDistance();
    float b = calculateDistance();
    float c = calculateDistance();

    float middleOfThree = fmaxf(fminf(a, b), fminf(fmaxf(a, b), c));

    printf("Middle distance is %.2f cm.\n", middleOfThree);

    return middleOfThree;
}

void cleanUp_hc_sr04()
{
    printf("Cleaning used hc-sr04 GPIO pins\n");
    pinMode(hc_sr04_TRIG, INPUT);
}
 
// Test functions
/*int main (int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: %s <trigger> <echo>\n\nWhere:\n- trigger is the wiringPi trigger pin number.\n- echo is the wiringPi echo pin number.\nUsing trigger %d and echo %d.\n", argv[0], argv[1], argv[2]);
        exit(EXIT_FAILURE);
    }

    printf("Raspberry Pi wiringPi HC-SR04 Sonar test program.\n");

    initialise_hc_sr04(atoi(argv[1]), atoi(argv[2])); // 7 0
        
    if (setuid(getuid()) < 0) 
    {
        perror("Dropping privileges failed.\n");
        exit(EXIT_FAILURE);
    }

    getMiddleOfThree();

    cleanUp_hc_sr04();

    return 0;
}*/