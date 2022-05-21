/*
Test code for reading binary input from IR sensor.
*/
#include "../Run_Code/pinout.h"
// #include "../Run_Code/Dispenser_lib.h"
#include "../Run_Code/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int fuck = 1;

void safe_terminate(int dummy) {
    fuck = 0;
    // gpioTerminate();
    gpioWrite(IRLED, 0);
    printf("pigpio terminated\n");
}

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

// Test code for displaying readings from IR transistor.
int main(){
    signal(SIGINT, safe_terminate);

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