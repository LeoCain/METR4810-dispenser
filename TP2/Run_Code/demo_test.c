#include "Dispenser_lib.h"
#include "Parameters.h"
#include "pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* File containing the main run code for the Dispenser project*/

void setup(){
    gpioInitialise();
    deactivate_segments();
    gpioSetMode(RollMot, PI_OUTPUT);
    gpioWrite(RollMot, 0);
    gpioSetMode(StepMot, PI_OUTPUT);
    gpioSetMode(DirStep, PI_OUTPUT);
    gpioSetMode(IR1, PI_INPUT);
    gpioSetMode(IR2, PI_INPUT);
    gpioSetMode(HAND, PI_INPUT);
    gpioSetMode(Doorservo, PI_OUTPUT);
    gpioSetMode(IRLED, PI_OUTPUT);
    gpioWrite(IRLED, 0);
}

int main(){
    setup();
    int first_LED = IR1;
    int last_LED = IR2;
    int curr_val = gpioRead(first_LED);
    while (curr_val == 1){
        curr_val = gpioRead(first_LED);
    }

    open_door();
    gpioWrite(RollMot, 1);
    printf("*** Mask loaded ***\n");
    
    while (curr_val == 0){
        gpioDelay(1000);
        curr_val = gpioRead(first_LED);
    }

    gpioWrite(RollMot, 0);
    printf("*** Mask ready for collection ***\n");

    int curr_val2 = gpioRead(last_LED);
    while (curr_val2 == 0){
        curr_val2 = gpioRead(last_LED);
    }

    printf("*** mask collected *** \n");
    close_door();

}