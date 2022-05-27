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
/**
 * TODO:
 * 5. make restocking code more coherent -- DONE
 * 6. more stable IR sensing function -- CHECK
 * 7. open door when IR2 tripped instead?
 * 9. detach stepper code -- DONE
 * 10. Fix flow charts -- DONE
 * 11. first mask pre-loaded -- FIX
 * 12: fix display update -- DONE
 * 13: update dual IR branch --DONE
 * 14: make SSD multithreading less shit -- DONE
 * 16: home -> detach shit -- CHECK
 * 17: fix vibration slip
 * 18: auto-rehome function (?)
 * 19: detach -> home when restocking -- CHECK
 * 20: make sure ALL required telemetry logs are there
 * 21: verify that new restock code works
 */

/* File containing the main run code for the Dispenser project */

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
    gpioSetMode(LEDs, PI_OUTPUT);
    gpioSetMode(STEP_SLP, PI_OUTPUT);
    gpioSetMode(HOME_RD, PI_INPUT);

    // set initial state of items
    gpioWrite(RollMot, 0);
    gpioWrite(DIR_PIN, TURN_DIRECTION);
    gpioWrite(STEP_SLP, 1);
    gpioWrite(LEDs, 0);
    
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

int dispenser(void);

int main(void) {
    // Init pigpio, set pinmodes, turn off LEDs, set start pos for servo
    // and move stepper to home position
    int reset_mode;
    setup();

    // If user sends SIGINT (cntrl-c), catch it, and terminate pigpio properly.
    signal(SIGINT, safe_terminate);

    printf("Enter: 0 -> detach then home. 1 -> only home. 2 -> only detach, anything else -> nothing: ");
    scanf("%d", &reset_mode);
    printf("%d Entered\n", reset_mode);

    if (reset_mode == 0) {
        detach_stepper(); // detach, wait till user presses enter, reattach
        home_stepper(); // Home stepper
    } else if (reset_mode == 1) {
        home_stepper(); // Home stepper
    } else if (reset_mode == 2) {
        detach_stepper(); // detach, wait till user presses enter, reattach
        gpioWrite(STEP_SLP, 1);
    }

    // Execute dispenser process
    dispenser();
    safe_terminate(0);
    return 0;
}

/**
 * Contains the state machine code for dispensor operation
 */
int dispenser(void){
    /* Request mask stock */
    char stock[9];
    printf("Please enter the stock of the dispenser: ");
    scanf("%s", stock);
    update_disp(stock);
    t_id_SSD = run_thread(1, " ");

    /* Begin state machine */
    printf("Waiting for mask request...\n");
    // INPUTS = {HAND, IR1, IR2, DOOR}
    int INPUTS[] = {presence_detect(HAND), presence_detect(IR1), presence_detect(IR2), 0};
    while(running2){
        // printf("find_state = %d\n", find_state(INPUTS));
        int current_state = find_state(INPUTS);
        // printf("%d", current_state);
        switch (current_state){
            case (0):
                // printf("INVALID STATE\n");
                break;
            case (1):
                // printf("ST1: Request_Wait\n");
                break;
            case (2):
                printf("ST2: Dropping_Mask\n");
                //turn on DISPENSE/IR LEDs
                gpioWrite(LEDs, 1);

                gpioWrite(RollMot, 1);
                turn(); // Rotate to next mask index:

                // After half a sec, vibe until mask drops
                // gpioDelay(500000);
                vibe_til_drop(stock);

                // If we reach this stage, the mask is sitting 
                // on top of rollers, ready to be fed out.
                // gpioDelay(1000);
                break;
            case (3):
                printf("ST3: Open_Door\n");
                open_door();
                INPUTS[3] = 1;
                printf("Door opened\n");
                break;
            case (4):
                printf("ST4: Feed_Mask\n");
                feed_til_fed(stock);
                /** If we reach this stage, the mask is fed out the door,
                 * ready for collection */
                break;
            case (5):
                printf("ST5: Take_Mask\n");
                // Wait for mask to be taken. if not taken, 
                // Err2 is displayed, mask jammed at door
                wait_for_take(stock);
                break;
            case (6):
                printf("ST6: Cleanup and return\n");
                /* Update the remaining stock: */
                int new_stock = atoi(stock) - 1;
                    // Convert to str and save as stock
                snprintf(stock, 9, "%d", new_stock);
                printf("New level of stock: %d\n", atoi(stock));
                    // Update display
                update_disp(stock);
                // wait 2 sec, close door, update door state, turn off green LED
                gpioDelay(1500000);
                close_door();
                INPUTS[3] = 0;
                gpioWrite(LEDs, 0);

                /* Check if stock depleted, if so prompt a refill then update stock */
                if (atoi(stock) <= 0) {
                    int reload_stock = restock_or_quit(stock);
                    if (reload_stock == 0) {
                        running2 = 0;
                        return 0; // CHECK THIS
                    }
                    snprintf(stock, 9, "%d", reload_stock);
                }
                update_disp(stock);
                gpioDelay(500000);
                printf("Waiting for mask request...\n");
                break;
        }
        INPUTS[0] = presence_detect(HAND);
        INPUTS[1] = presence_detect(IR1);
        INPUTS[2] = presence_detect(IR2);
    }
    return 1;
}













/* Handles when user sends SIGINT (cntrl-c) */
// void terminate_handler(int dummy) {
//     int usr_in;
//     printf("SIGINT sent. Enter 0 for Full Reset. Enter 1 for Quit. Enter 2 for Partial Reset: \n");
//     scanf("%d", &usr_in);
//     switch(usr_in) {
//         case (0):
//             // Full Reset
//             printf("Full Reset sent\n");
//             safe_terminate();
//             main();
        
//         case (1):
//             // Quit
//             printf("Quit command sent\n");
//             safe_terminate();
//             _Exit(1);

//         case (2):
//             // Partial reset
//             gpioWrite(STEP_SLP, 0);
//             printf("Mag detatched, Partial Reset sent, press enter to continue\n");
//             // scanf("%d", &usr_in);
//             gpioWrite(STEP_SLP, 1);
//             printf("Stepper reattached");
//             dispenser();
        
//         default:
//             printf("Unhandled input, terminating...\n");
//             safe_terminate();
//             _Exit(1);
//     }

// }

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