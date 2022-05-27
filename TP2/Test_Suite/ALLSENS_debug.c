#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include "../Headers/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
/* debugging for IRLED */

void setup(){
    gpioInitialise();

    deactivate_segments();
    gpioSetMode(RollMot, PI_OUTPUT);
    gpioSetMode(STEP_PIN, PI_OUTPUT);
    gpioSetMode(DIR_PIN, PI_OUTPUT);
    gpioSetMode(IR1, PI_INPUT);
    gpioSetMode(IR2, PI_INPUT);
    gpioSetMode(HAND, PI_INPUT);
    gpioSetMode(Doorservo, PI_OUTPUT);
    gpioSetMode(LEDs, PI_OUTPUT);
    gpioSetMode(STEP_SLP, PI_OUTPUT);
    gpioSetMode(HOME_RD, PI_INPUT);

    // set initial state of items
    gpioWrite(RollMot, 0);
    gpioWrite(DIR_PIN, TURN_DIRECTION);
    gpioWrite(STEP_SLP, 1);
    gpioWrite(LEDs, 1);
    
    // change this to the close position
    gpioServo(Doorservo, CLOSE);
    sleep(1);
    open_door();
    close_door();

    // Initialise multithreading
    SSDon = 0;
    pthread_mutex_init(&lock, NULL);
    
    // initialise stepper vars
    tick = 0;
	step_to_turn = (unsigned)(TOTAL_STEP / SEGMENTS * 2);
	step_delay_us = STEP_DELAY_MS * 1000;
	vibrate_delay_us = VIBRATE_DELAY_MS * 1000;
}

int main(void) {
    setup();
    signal(SIGINT, safe_terminate);
    gpioWrite(STEP_SLP, 1);
    home_stepper();
    while (running2) {
        printf("IR1: %d, IR2 %d, HAND: %d, HOME: %d\n", presence_detect(IR1), presence_detect(IR2), presence_detect(HAND), gpioRead(HOME_RD));
    }
    return 0;
}