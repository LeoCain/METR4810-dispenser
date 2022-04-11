/* 
Test code for the stepper motor. Requires the ____ stepper motor driver.
Was tested on a NEMA17 stepper motor. Moves the motor 10 steps.
*/
#include "../Run_Code/pinout.h"
#include "../Run_Code/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void step_mag(int steps, int direction) {
    gpioWrite(DirStep, direction);
    for (int i=0; i<steps; i++) {
        clock_t time = clock();
        while ((clock()-time)< spin_delay) {}
        int curr_level = gpioRead(StepMot);
        printf("%d\n", curr_level);
        if (curr_level == 1) {
            gpioWrite(StepMot, 0);
        } else if (curr_level == 0) {
            gpioWrite(StepMot, 1);
        } else {
            printf("Error, bad StepMot GPIO");
        }
    }
}
int main() {
    gpioInitialise();
    gpioSetMode(StepMot, PI_OUTPUT);
    gpioSetMode(DirStep, PI_INPUT);
    while(1){
        step_mag(10, 1);
    }

    gpioTerminate();
}