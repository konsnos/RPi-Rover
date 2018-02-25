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
    hc_sr04_initialise(7, 0);
    l298n_initialise(2, 3, 21, 22);
}

/**
 * Cleans up used pins to avoid any damage.
 **/
void cleanUp()
{
    hc_sr04_cleanUp();
    l298n_cleanUp();
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

    l298n_forward();

    // initialise distances
    // length of 3 was tested but there were a lot of false positives.
    distances[0] = hc_sr04_getMiddleOfThree();
    distances[1] = hc_sr04_getMiddleOfThree();
    distances[2] = hc_sr04_getMiddleOfThree();
    distances[3] = hc_sr04_getMiddleOfThree();
    distances[4] = hc_sr04_getMiddleOfThree();

    while(keepRunningProgram)
    {
        // get distance from an obstacle
        distances[distancesCurIndex] = hc_sr04_getMiddleOfThree();

        if(movingForward)
        {
            // if moving forward and distance is less than 10 cm, then stop and turn.
            if(distances[distancesCurIndex] < 20.0)
            {
                movingForward = 0;
                l298n_stop();
                delay(STOP_DELAY);
                l298n_reverse();
                delay(REVERSE_DELAY);
                
                if(turnRight)
                    l298n_turn_right();
                else
                    l298n_turn_left();
                delay(TURN_DELAY);
                l298n_stop();
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
                    l298n_stop();
                    delay(STOP_DELAY);
                    l298n_reverse();
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
                if(turnRight)
                    l298n_turn_right();
                else
                    l298n_turn_left();
                delay(TURN_DELAY);
                l298n_stop();
                forceTurn = 0;
            }
            else
            {
                // no obstacle found, keep going!
                l298n_forward();
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