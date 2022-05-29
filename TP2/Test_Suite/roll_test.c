/*
Test code for running the DC motor via BJT
*/
#include "../Headers/pinout.h"
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    setup();
    signal(SIGINT, safe_terminate);
    
    gpioWrite(RollMot, 1); // turn on motor pin
    sleep(10);
    gpioWrite(RollMot, 0);
    gpioTerminate();
}