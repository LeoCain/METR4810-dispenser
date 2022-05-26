/*
Test code for testing the IR obstacle detection sensor
*/
#include "../Headers/pinout.h"
#include "../Headers/Parameters.h"
#include "../Headers/Dispenser_lib.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(){
    gpioInitialise();
    gpioSetMode(HAND, PI_INPUT);
    while (!presence_detect(HAND)){
        printf("%d", presence_detect(HAND));
        printf("%d\n", gpioRead(HAND));
    }

}