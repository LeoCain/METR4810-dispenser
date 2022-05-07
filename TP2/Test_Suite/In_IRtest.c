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
    float IR_val;
    for(int i=0; i>-1; i++){
        IR_val = gpioRead(IR1);
        printf("Current IR val: %f\n", IR_val);
    }
    gpioTerminate();    //Terminate pigpio
}