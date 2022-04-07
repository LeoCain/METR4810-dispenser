// #include "/home/ubuntu/TP2/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    // // define sev seg display pins
    // int seg_list[] = {SevSegE, SevSegD, SevSegC, SevSegG, SevSegF, SevSegA};
    // int dig_list[] = {SevSegD1, SevSegD2, SevSegD3, SevSegD4};

    // //initialise pins
    // gpioInitialise();
    // for (int i=0; i<6; i++){
    //     gpioSetMode(seg_list[i], PI_OUTPUT);
    //     gpioWrite(seg_list[i], 1); //deactivate all segs
    // }
    // for (int i=0; i<4; i++){
    //     gpioSetMode(dig_list[i], PI_OUTPUT);
    //     gpioWrite(dig_list[i], 0); //deactivate all digits
    // }

    // // lets try light some up
    // gpioWrite(SevSegD4, 1); //activate digit 1
    // gpioWrite(SevSegF, 0); // turn on segment 
    // sleep(4);
    // gpioTerminate();
    // char bruh[] = "4.kys";
    // printf("%ld", (sizeof bruh / sizeof bruh[0]) - 1);
    for (int i=0; i<=100; i+=10) {
        printf("%d", i);
    }
}