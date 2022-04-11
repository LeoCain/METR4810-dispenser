/*
Test code for running the DC motor via BJT
*/
#include "../Run_Code/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    gpioInitialise(); // initialise pigpio
    gpioSetMode(RollMot, PI_OUTPUT); // set motor pin as an output
    clock_t time = clock();
    while((clock() - time) < 2000000){
        gpioWrite(RollMot, 1); // turn on motor pin
    }
    gpioWrite(RollMot, 0);
    gpioTerminate();
}