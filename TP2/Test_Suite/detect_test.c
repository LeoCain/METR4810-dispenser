#include "../pinout.h"
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
    if (avg == 1){
        return 1;
    } else {
        return 0;
    }

}
int main(){
    gpioInitialise();
    gpioSet(HAND, PI_INPUT);
    while (1){
        printf("%d", hand_present());
    }

}