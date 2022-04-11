/*
Test code for testing the IR obstacle detection sensor
*/
#include "../Run_Code/pinout.h"
#include "../Run_Code/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int hand_present(){
    float avg = 0;
    int sample = 25;
    for (int i=0; i<sample; i++) {
        avg += gpioRead(HAND);
    }
    avg = avg/sample;
    if (avg == hand_val){
        return 1;
    } else {
        return 0;
    }

}
int main(){
    gpioInitialise();
    gpioSetMode(HAND, PI_INPUT);
    while (1){
        printf("%d\n", hand_present(HAND));
    }

}