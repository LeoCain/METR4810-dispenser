#include "Dispenser_lib.h"
#include "Parameters.h"
#include "pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
/**
 * TODO list:
 * 1. Sometimes doesnt terminate properly --- SEEMS to be fixed
 * 2. cmd thread doesnt seem to restart after restocking --- SEEMS to be fixed
 * 3. sometimes freezes --- SEEMS to be fixed
 * 4. currently no means of resetting, cmd thread does almost nothing
 * 5. make restocking code more coherent
 */
/* File containing the main run code for the Dispenser project*/
// initialise global vars
int SSDon;
static volatile int running = 1;
static volatile int running2 = 1;
long unsigned int t_id_SSD;
long unsigned int t_id_cmd;
pthread_mutex_t lock;
pthread_mutex_t lock2;

/**
 * Used to safely terminate threads and pigpio
 */
void terminate_handler(int dummy) {
    running = 0;
    running2 = 0;
    printf("Terminated\n");
    if (dummy != 0) {
        printf("SIGINT detected, Terminating...\n");
    }
    // ensure SSD is off
    SSDon = 0;
    // Ensure thread is closed
    pthread_cancel(t_id_cmd);
    pthread_cancel(t_id_SSD);
    printf("Threads finished\n");
    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock2);
    printf("mutex unlocked\n");
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock2);
    printf("mutex destroyed\n");
    // Join threads
    pthread_join(t_id_cmd, NULL);
    pthread_join(t_id_SSD, NULL);
    printf("Threads joined\n");
    gpioTerminate();
    printf("pigpio terminated\n");
    _Exit(1);
}

/**
 * Sets pin modes, initial pin states/positions,
 * and initialises important variables
 */
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
    gpioSetMode(IRLED, PI_OUTPUT);
    // set initial state of items
    gpioWrite(RollMot, 0);
    gpioWrite(IRLED, 1);
    gpioWrite(DIR_PIN, TURN_DIRECTION);
    // change this to the close position
    gpioServo(Doorservo, OPEN);
    sleep(1);
    close_door();

    // Initialise multithreading
    SSDon = 0;
    pthread_mutex_init(&lock, NULL);
    
    // initialise stepper vars
    tick = 0;
	step_to_turn = (unsigned)(TOTAL_STEP / SEGMENTS * 2);
	step_delay_us = STEP_DELAY_MS * 1000;
	vibrate_delay_us = VIBRATE_DELAY_MS * 1000;

    // add code here to move stepper to homing position
}

void dispenser();

int main(void) {
    // Init pigpio, set pinmodes, turn off LEDs, set start pos for servo
    // and move stepper to home position
    setup();
    // If user sends SIGINT (cntrl-c), catch it, and terminate pigpio properly.
    signal(SIGINT, terminate_handler);
    // Execute dispenser process
    // while (running) { dispenser(); }
    dispenser();
    return 0;
}

/**
 * Contains the state machine code for dispensor operation
 */
void dispenser(){
    /* Request mask stock */
    char stock[8];
    printf("Please enter the stock of the dispenser: ");
    scanf("%s", stock);
    t_id_SSD = run_thread(0, stock);

    /* Launch cmd thread */
    t_id_cmd = run_thread(1, "");

    /* Begin state machine */
    printf("Waiting for mask request...\n");
    int INPUTS[] = {hand_present(), gpioRead(IR1), gpioRead(IR2), 0};
    while(running2){
        // printf("find_state = %d\n", find_state(INPUTS));
        int current_state = find_state(INPUTS);
        switch (current_state){
            case (0):
                // printf("INVALID STATE\n");
                break;
            case (1):
                // printf("ST1: Request_Wait\n");
                break;
            case (2):
                printf("ST2: Dropping_Mask\n");
                // TODO: add code for illuminating green LED
                
                turn(); // Rotate to next mask index:

                // After half a sec, vibe until mask drops
                gpioDelay(500000);
                t_id_SSD = vibe_til_drop(t_id_SSD, stock);

                // If we reach this stage, the mask is sitting 
                // on top of rollers, ready to be fed out.
                gpioDelay(1000);
                break;
            case (3):
                printf("ST3: Open_Door\n");
                open_door();
                INPUTS[3] = 1;
                printf("Door opened\n");
                break;
            case (4):
                printf("ST4: Feed_Mask\n");
                t_id_SSD = feed_til_fed(t_id_SSD, stock);
                /** If we reach this stage, the mask is fed out the door,
                 * ready for collection */
                break;
            case (5):
                printf("ST5: Take_Mask\n");
                // Wait for mask to be taken. if not taken, 
                // Err2 is displayed, mask jammed at door
                t_id_SSD = wait_for_take(t_id_SSD, stock);
                break;
            case (6):
                printf("ST6: Cleanup and return\n");
                /* Update the remaining stock: */
                int new_stock = atoi(stock) - 1;
                    // Convert to str and save as stock
                snprintf(stock, 8, "%d", new_stock);
                    // Update display
                SSDon = 0;
                pthread_join(t_id_SSD, NULL);
                t_id_SSD = run_thread(0, stock);
                // wait 2 sec, close door, update door state, turn off green LED
                gpioDelay(2000000);
                close_door();
                INPUTS[3] = 0;

                /* Check if stock depleted, if so prompt a refill then update stock */
                int reload_stock = restock_or_quit(t_id_cmd, stock);
                snprintf(stock, 8, "%d", reload_stock);
                
                // TODO: add code to turn off LED
                if (reload_stock == 0 ){
                    running2 = 0;
                } 
                if (running2) {
                    if (!(reload_stock == new_stock)) {
                        t_id_cmd = run_thread(1, "4201"); // restart cmd thread
                    }
                    printf("Waiting for mask request...\n");
                }
                break;
        }
        INPUTS[0] = hand_present();
        INPUTS[1] = gpioRead(IR1);
        INPUTS[2] = gpioRead(IR2);
    }
    terminate_handler(0);
}


                // /* Check if stock depleted */
                // if (atoi(stock) <= 0) {
                //     // TODO: add code here to return stepper to home
                //     // Command centre is using scanf, cancel it's thread:
                //     pthread_cancel(t_id_cmd);
                //     printf("cmd thread cancelled\n");
                //     pthread_mutex_unlock(&lock2);
                //     printf("mutex unlocked\n");
                //     pthread_join(t_id_cmd, NULL);
                //     printf("cmd thread joined\n");
                //     // Wait to be restocked
                //     printf("Stock depleted. Please refill\n");
                //     printf("Enter number of masks restocked, or F to exit process: ");
                //     scanf("%s", stock);
                //     printf("you entered: %d\n", atoi(stock));
                //     int valid_input = 0;
                //     while (!valid_input){
                //         if (strchr(stock, 'F')){
                //             printf("\nexiting...");
                //             valid_input = 1;
                //             running2 = 0;
                //         } else if (!(atoi(stock)>=1 && atoi(stock)<=19)){
                //             printf("\nInvalid input, please try again: ");
                //             scanf("%s", stock);
                //         } else {
                //             valid_input = 1;
                //         }
                //     }
                // }