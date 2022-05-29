/*
Test code for spinning an MG-90 (Metal-gear SG-90)
should spin to max CCW, max CW
*/
#include "../Headers/pinout.h"
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void old_design(){
    gpioServo((unsigned) 12, (unsigned) 1000); // Move servo to safe position anti-clockwise.
    sleep(1);
    gpioServo((unsigned) 12, (unsigned) 1500); // Move servo to centre position.
    sleep(1);    
    gpioServo(12, 2000); // Move servo to safe position clockwise.
    sleep(1);
}

int main(){
    setup();
    signal(SIGINT, safe_terminate);

    while(running2){
        gpioServo(Doorservo, CLOSE);
        sleep(2);
        open_door();
        // gpioServo(Doorservo, 1000);
        printf("open\n");
        sleep(1);
        // gpioServo(Doorservo, 2000);
        close_door();
        printf("close\n");
        sleep(1);
    }

    gpioTerminate();

}