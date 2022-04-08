#include "../Run_Code/pinout.h"
#include "../Run_Code/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

void step_mag(int steps, int direction) {
    gpioWrite(DirStep, direction);
    for (int i=0; i<steps; i++) {
        int curr_level = gpioRead(StepMot);
        if (curr_level) {
            gpioWrite(StepMot, 0);
        } else {
            gpioWrite(StepMot, 1);
        }
    }
}
int main() {
    gpioInitialise();
    step_mag(10, 1);
    gpioTerminate();
}