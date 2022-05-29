/*
Test code for blinking an LED
*/
#include "../Headers/pinout.h"
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>     

int main(void) {
    setup();
    signal(SIGINT, safe_terminate);
    int i = 0;
    while (running2) {
        gpioWrite(LEDs, 1); // turn on LED
        sleep(1); // wait 1 second
        gpioWrite(LEDs, 0);
        sleep(1); // wait 1 second
        i++;
    }
    gpioTerminate();
}


