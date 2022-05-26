/*
Test code for blinking an LED
*/
#include "../Headers/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    gpioInitialise(); // initialise pigpio
    gpioSetMode(12, PI_OUTPUT); // set LED pin as an output
    int i = 0;
    while (i < 100) {
        gpioWrite(12, 1); // turn on LED
        sleep(1); // wait 1 second
        // gpioWrite(12, 0);
        sleep(1); // wait 1 second
        i++;
    }
    gpioTerminate();
}


