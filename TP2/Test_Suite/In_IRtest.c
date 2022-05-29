/*
Test code for reading binary input from IR sensor.
*/
#include "../Headers/pinout.h"
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static volatile int ree = 1;

// void safe_terminate_IR(int dummy) {
//     ree = 0;
//     // gpioTerminate();
//     gpioWrite(LEDs, 0);
//     printf("pigpio terminated\n");
//     _Exit(1);
// }

// Test code for displaying readings from IR transistor.
int main(){
    setup();
    signal(SIGINT, safe_terminate);
    gpioWrite(LEDs, 1);

    int IR1_val;
    int IR2_val;
    int i = 0;
    while (running2){
        IR1_val = presence_detect(IR1);
        IR2_val = presence_detect(IR2);
        printf("IR1: %d, ", IR1_val);
        printf("IR2: %d\n", IR2_val);
        gpioDelay(10000);
        i++;
    }
    gpioWrite(LEDs, 0);
    gpioTerminate();    //Terminate pigpio
}