#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include "../Headers/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
/* debugging for IRLED */

int main(void) {
    setup();
    signal(SIGINT, safe_terminate);
    gpioPWM(LEDs, 250);
    gpioWrite(STEP_SLP, 1);
    // home_stepper();
    while (running2) {
        printf("IR1: %d, IR2 %d, HAND: %d, HOME: %d\n", presence_detect(IR1), presence_detect(IR2), presence_detect(HAND), gpioRead(HOME_RD));
    }
    return 0;
}