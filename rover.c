/**
 * Drives a rover avoiding obstacles with an L298N Dual H Bridge controller for the dc motors and HC-SR04 Ultrasonic Sensor to sense obstacles.
 **/

#include "l298n/l298n.h"
#include "hc-sr04/hc-sr04.h"

#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>

int STOP_DELAY = 200;
int TURN_DELAY =  700;
int REVERSE_STACK_DELAY = 1000;
int REVERSE_DELAY = 500;

static volatile int keepRunningProgram = 1;

/**
 * Ctrl+C pressed. Stop running program.
 **/
void intHandler(int dummy)
{
    keepRunningProgram = 0;
}

/**
 * Initialise libraries used with their WiringPi numbering scheme ( http://wiringpi.com/pins/ ).
 **/
void initialisation()
{
    initialise_hc_sr04(7, 0);
    initialise_l298n(2, 3, 21, 22);
}

/**
 * Cleans up used pins to avoid any damage.
 **/
void cleanUp()
{
    cleanUp_hc_sr04();
    cleanUp_l298n();
}

/**
 * Drive forward.
 * If any obstacles are found then turn.
 * If an obstacle is scanned in the same distance after going forward then we are stuck! Try to reverse and turn.
 **/
void drive()
{
    int turnRight = 1;
    int movingForward = 1;
    /* Forces the rover to turn. Should be used after reverse. */
    int forceTurn = 0;
    /* Last three distances measured. If they are approximately no farther than 1 cm. then the rover is stuck somewhere. */
    int DISTANCES_SIZE = 5;
    float distances[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    /* Current index of distances to register. */
    int distancesCurIndex = 0;

    forward();

    // initialise distances
    // length of 3 was tested but there were a lot of false positives.
    distances[0] = getMiddleOfThree();
    distances[1] = getMiddleOfThree();
    distances[2] = getMiddleOfThree();
    distances[3] = getMiddleOfThree();
    distances[4] = getMiddleOfThree();

    while(keepRunningProgram)
    {
        // get distance from an obstacle
        distances[distancesCurIndex] = getMiddleOfThree();

        if(movingForward)
        {
            // if moving forward and distance is less than 10 cm, then stop and turn.
            if(distances[distancesCurIndex] < 20.0)
            {
                movingForward = 0;
                stop();
                delay(STOP_DELAY);
                reverse();
                delay(REVERSE_DELAY);
                
                if(turnRight)
                    turn_right();
                else
                    turn_left();
                delay(TURN_DELAY);
                stop();
            }
            else
            {
                // else check if stuck by measuring all distances
                if(abs(distances[0] - distances[1]) < 1.0 &&
                   abs(distances[1] - distances[2]) < 1.0 &&
                   abs(distances[2] - distances[3]) < 1.0 &&
                   abs(distances[3] - distances[4]) < 1.0 &&
                   abs(distances[4] - distances[0]) < 1.0
                )
                {
                    movingForward = 0;
                    stop();
                    delay(STOP_DELAY);
                    reverse();
                    delay(REVERSE_STACK_DELAY);
                    forceTurn = 1;
                }
            }
        }
        else
        {
            // After a turn keep scanning for obstacles to continue turning.
            if(distances[distancesCurIndex] < 30.0 || forceTurn)
            {
                // keep turning
                delay(STOP_DELAY);
                if(turn_right)
                    turn_right();
                else
                    turn_left();
                delay(TURN_DELAY);
                stop();
                forceTurn = 0;
            }
            else
            {
                // no obstacle found, keep going!
                forward();
                movingForward = 1;
                turnRight = turnRight?0:1;
            }
        }
        distancesCurIndex++;
        if(distancesCurIndex == DISTANCES_SIZE)
            distancesCurIndex = 0;
    }
}

int main (int argc, char *argv[])
{
    // Listen to Ctrl+C
    signal(SIGINT, intHandler);

    initialisation();

    drive();

    cleanUp();

    return 0;
}