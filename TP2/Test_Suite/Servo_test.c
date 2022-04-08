/*
Test code for spinning an MG-90 (Metal-gear SG-90)
should spin to max CCW, middle, max CW
*/
#include "../Run_Code/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>


int main(){
    gpioInitialise();
    gpioSetMode(Doorservo, PI_OUTPUT); // set Doorservo pin as output
    printf("Anticlockwise\n");
    gpioServo(Doorservo, 1000);
    sleep(3);
    printf("Centre\n");
    gpioServo(Doorservo, 1500);
    sleep(2);
    printf("Clockwise\n");
    gpioServo(Doorservo, 2000);
    sleep(2);
    gpioTerminate();

}