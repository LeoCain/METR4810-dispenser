/*
Test code for testing the IR obstacle detection sensor
*/
#include "../Headers/pinout.h"
#include "../Headers/Parameters.h"
// #include "../Headers/Dispenser_lib.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int presence_detect(int sensor_pin){
    float tot = 0;
    int sample = 50;
    float avg;
    for (int i=0; i<sample; i++) {
        tot += gpioRead(sensor_pin);
    }
    avg = tot/sample;

    if (avg == 1 && avg == gpioRead(sensor_pin)){
        return 1;
    } else{
        return 0;
    }
}

int main(){
    gpioInitialise();
    gpioSetMode(HAND, PI_INPUT);
    while (!presence_detect(HAND)){
        printf("%d", presence_detect(HAND));
        printf("%d\n", gpioRead(HAND));
    }

}