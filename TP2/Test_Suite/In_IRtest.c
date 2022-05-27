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

static volatile int fuck = 1;

void safe_terminate_IR(int dummy) {
    fuck = 0;
    // gpioTerminate();
    gpioWrite(LEDs, 0);
    printf("pigpio terminated\n");
    _Exit(1);
}

// Test code for displaying readings from IR transistor.
int main(){
    signal(SIGINT, safe_terminate_IR);

    gpioInitialise();   // initialise pigpio
    gpioSetMode(IR1, PI_INPUT); // set IR transistor pin as input
    gpioSetMode(IR2, PI_INPUT);
<<<<<<< HEAD
    gpioSetPullUpDown(IR1, PI_PUD_DOWN);
    gpioSetPullUpDown(IR2, PI_PUD_DOWN);
    gpioSetMode(IRLED, PI_OUTPUT);
    gpioWrite(IRLED, 1);
=======
    gpioSetMode(LEDs, PI_OUTPUT);
    gpioWrite(LEDs, 1);
>>>>>>> cc213aebdf67fdc75c3f39cfe78835e27042c860
    int IR1_val;
    int IR2_val;
    int i = 0;
    while (1){
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