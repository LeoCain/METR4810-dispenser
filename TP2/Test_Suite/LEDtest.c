/*
Test code for blinking an LED
*/
#include "../Headers/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    gpioInitialise(); // initialise pigpio
<<<<<<< HEAD
    gpioSetMode(12, PI_); // set LED pin as an output
=======
    gpioSetMode(IR1, PI_OUTPUT); // set LED pin as an output
>>>>>>> cc213aebdf67fdc75c3f39cfe78835e27042c860
    int i = 0;
    while (i < 100) {
        gpioWrite(IR1, 0); // turn on LED
        sleep(5); // wait 1 second
        gpioWrite(IR1, 0);
        sleep(5); // wait 1 second
        i++;
    }
    gpioTerminate();
}


