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

int fuck = 1;

void safe_terminate_IR(int dummy) {
    fuck = 0;
    // gpioTerminate();
    gpioWrite(IRLED, 0);
    printf("pigpio terminated\n");
}

// Test code for displaying readings from IR transistor.
int main(){
    signal(SIGINT, safe_terminate_IR);

    gpioInitialise();   // initialise pigpio
    gpioSetMode(IR1, PI_INPUT); // set IR transistor pin as input
    gpioSetMode(IR2, PI_INPUT);
    gpioSetMode(IRLED, PI_OUTPUT);
    gpioWrite(IRLED, 1);
    int IR1_val;
    int IR2_val;
    int i = 0;
    while (i < 1000){
        IR1_val = presence_detect(IR1);
        IR2_val = presence_detect(IR2);
        printf("IR1: %d, ", IR1_val);
        printf("IR2: %d\n", IR2_val);
        gpioDelay(10000);
        i++;
    }
    gpioWrite(IRLED, 0);
    gpioTerminate();    //Terminate pigpio
}