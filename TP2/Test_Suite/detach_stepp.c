#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include "../Headers/pinout.h"

int main(void){
    gpioInitialise();
    gpioSetMode(STEP_SLP, PI_OUTPUT);
    gpioWrite(STEP_SLP, 0);
    getchar();
    gpioTerminate();
}