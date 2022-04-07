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
    setup();    // set pinmodes and initial states.
    int stock;
    printf("Please enter the stock of the dispenser: ")
    scanf("%d", &stock);
    for (int i=0; i<stock; i++) {
        // Wait for hand to be detected:
        hand_presence = hand_present(gpioRead(HAND));
        while (hand_presence != hand_val) {
            hand_presence = hand_present(gpioRead(HAND));
        }
        printf("Customer mask request detected...loading mask...\n")
        /* A hand has been detected -> begin load process */

        // Illuminate IR LEDs and Green LED:
        gpioWrite(IRLED, 1);
        // Step stepper motor 
        step_mag(next_mask, step_CCW);
        // TODO: START MASK DROP TIMER HERE
        // Wait for mask to fall down to roller
        LOAD_val = gpioRead(IR1);
        while (LOAD_val == 1) {
            LOAD_val = gpioRead(IR1);
            // TODO: CHECK MASK DROP TIMER - if too long, display Err0 on SSD
        }
        printf("Mask loaded...dispensing...\n")

        /* Mask is now sitting on rollers - begin dispense process */

        // Open sliding door
        open_door();
        // Switch on roller motor
        gpioWrite(RollMot, 1);
        // TODO: START ROLL TIMER HERE
        // Wait for mask to reach customer acquisition position:
        LOAD_val = gpioRead(IR1);
        while (LOAD_val == 0) {
            LOAD_val = gpioRead(IR1);
            // TODO: CHECK ROLL TIMER - if too long, display Err1 on SSD
        }
        // Stop roller motor
        gpioWrite(RollMot, 0);
        printf("Mask dispensed and ready for collection.\n")

        /* mask is now waiting to be collected by customer */

        // Wait for mask to be taken
        // TODO: START COLLECTION TIMER HERE
        DISPENSE_val = gpioRead(IR2);
        while (DISPENSE_val == 0) {
            DISPENSE_val = gpioRead(IR1);
            // TODO: CHECK COLLECTION TIMER - if too long, display Err2 on SSD
        }
        // Mask is taken, de-illuminate green LED
        gpioWrite(IRLED, 1);
        printf("Mask has been collected. Remaining stock: %d\n", stock-1-i)
    }
    printf("*****Stock depleted. Please refill*****")
    gpioTerminate();

}