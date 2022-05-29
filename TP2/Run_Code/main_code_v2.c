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
 * 11. first mask pre-loaded -- FIX
 * 17: fix vibration slip
 * 18: auto-rehome function (?)
 * 19: detach -> home when restocking -- CHECK
 * 20: make sure ALL required telemetry logs are there --DONE?
 * 21: verify that new restock code works
 * 22: fix weird skipping out of feed function
 * 23: Err3 display -- CHECK
 * 24: Fix safe terminate in test code --CHECK
 */

/* File containing the main run code for the Dispenser project */

int dispenser(void);

int main(void) {
    // Init pigpio, set pinmodes, turn off LEDs, set start pos for servo
    // and move stepper to home position
    int reset_mode;
    main_setup();

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
            case (UNDEF_STATE):
                /*** State is not defined ***/
                update_disp("Err3");
                break;

            case (WAIT):
                /*** Waiting for mask request ***/
                break;

            case (DROP_MASK):
                /*** Dropping mask process ***/
                printf("ST2: Dropping_Mask... ");

                //turn on DISPENSE/IR LEDs
                gpioPWM(LEDs, LED_BRIGHTNESS);
                // gpioWrite(LEDs, 1);

                gpioWrite(RollMot, 1); // Start spinning rollers
                turn(); // Rotate to next mask index:

                // Vibrate magazine until mask drops
                vibe_til_drop(stock);

                printf("Mask dropped\n");
                // If we reach this stage, the mask is sitting 
                // on top of rollers, ready to be fed out.
                break;

            case (OPEN_DOOR):
                /*** Open door process ***/
                printf("ST3: Open_Door... ");
                open_door();
                INPUTS[3] = 1; // sets door_open flag
                printf("Door opened\n");
                break;

            case (FEED_MASK):
                /*** Mask feed process ***/
                printf("ST4: Feed_Mask... ");
                // Continue spinning rollers until IR1 is unblocked
                // -> IR1 unblocked when mask positioned for taking
                feed_til_fed(stock);
                /** If we reach this stage, the mask is fed out the door,
                 * ready for collection */
                printf("Mask fed\n");
                break;
                
            case (TAKE_MASK):
                /*** Mask take process ***/
                printf("ST5: Take_Mask... ");
                // Wait for mask to be taken. if not taken, 
                // Err2 is displayed; mask jammed at door
                wait_for_take(stock);
                printf("Mask delivered\n");
                break;

            case (FINISH):
                /*** Finish/close/restock process ***/
                printf("ST6: Cleanup and return... ");

                /* Update the remaining stock: */
                int new_stock = atoi(stock) - 1;
                snprintf(stock, 9, "%d", new_stock);

                // wait a moment, close door, update door state, turn off green LED
                gpioDelay(1500000);
                close_door();
                INPUTS[3] = 0;
                gpioWrite(LEDs, 0);
                
                /* Display stock level */
                printf("New level of stock: %d\n", atoi(stock));
                update_disp(stock);

                /* Check if stock depleted, if so prompt a refill then update stock */
                if (atoi(stock) <= 0) {
                    int reload_stock = restock_or_quit(stock);
                    if (reload_stock == 0) {
                        running2 = 0;
                        return 0; // CHECK THIS
                    }
                    snprintf(stock, 9, "%d", reload_stock);
                    printf("Dispenser reloaded to a stock of: %d\n", atoi(stock));
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
