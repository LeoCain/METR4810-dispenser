#include "/home/ubuntu/TP2/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

// Test code for spinning an MG-90 (Metal-gear SG-90)
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