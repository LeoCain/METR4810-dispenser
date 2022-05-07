/*
Test code for reading binary input from IR sensor.
*/
#include "../Run_Code/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

// Test code for displaying readings from IR transistor.
int main(){
    gpioInitialise();   // initialise pigpio
    gpioSetMode(IR1, PI_INPUT); // set IR transistor pin as input
    gpioSetMode(IR2, PI_INPUT);
    gpioSetMode(IRLED, PI_OUTPUT);
    gpioWrite(IRLED, 1);
    int IR1_val;
    int IR2_val;
    for(int i=0; i>-1; i++){
        IR1_val = gpioRead(IR1);
        IR2_val = gpioRead(IR2);
        printf("IR1: %d, ", IR1_val);
        printf("IR2: %d\n", IR2_val);
    }
    gpioTerminate();    //Terminate pigpio
}