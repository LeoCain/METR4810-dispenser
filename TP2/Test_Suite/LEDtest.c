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
<<<<<<< HEAD
    while (i < 100) {
        gpioWrite(IR1, 0); // turn on LED
        sleep(1); // wait 1 second
        gpioWrite(IR1, 0);
=======
    while (running2) {
        gpioWrite(LEDs, 1); // turn on LED
        sleep(1); // wait 1 second
        gpioWrite(LEDs, 0);
>>>>>>> 0cb0f80de062f081e34372a8af1856c8f8463a3c
        sleep(1); // wait 1 second
        i++;
    }
    gpioTerminate();
}


