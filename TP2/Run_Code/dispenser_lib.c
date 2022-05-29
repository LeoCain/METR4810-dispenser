/*
Src file for the dispenser library
*/
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include "../Headers/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// values of different numbers/characters for seven seg display
char zero[] = {0, 0, 0, 1, 0, 0, 0, 1};
char one[] = {1, 1, 0, 1, 0, 1, 1, 1};
char two[] = {0, 0, 1, 0, 0, 1, 0, 1};
char three[] = {1, 0, 0, 0, 0, 1, 0, 1};
char four[] = {1, 1, 0, 0, 0, 0, 1, 1};
char five[] = {1, 0, 0, 0, 1, 0, 0, 1};
char six[] = {0, 0, 0, 0, 1, 0, 0, 1};
char seven[] = {1, 1, 0, 1, 0, 1, 0, 1};
char eight[] = {0, 0, 0, 0, 0, 0, 0, 1};
char nine[] = {1, 0, 0, 0, 0, 0, 0, 1};
char E[] = {0, 0, 1, 0, 1, 0, 0, 1};
char r[] = {0, 1, 1, 0, 1, 1, 1, 1};

// Lists containing the seven seg pins in correct order
int seg_list[] = {SevSegE, SevSegD, SevSegC, SevSegG, SevSegB, SevSegF, SevSegA, SevSegDP};
int dig_list[] = {SevSegD1, SevSegD2, SevSegD3, SevSegD4};

/* Turns off all LEDs on the the SSD */
void deactivate_segments(){
    /* sets pinmode for each ssd pin, and initialises them as off */
    for (int i=0; i<8; i++){
        gpioSetMode(seg_list[i], PI_OUTPUT);
        gpioWrite(seg_list[i], 1); // deactivate all segs
    }
    for (int i=0; i<4; i++){
        gpioSetMode(dig_list[i], PI_OUTPUT);
        gpioWrite(dig_list[i], 0); //deactivate all digits
    }
}

/**
 * Displays the given string of chars to the SSD.
 * only supports up to 4-digit numbers and Err codes.
 */
void *SS_print(void *no) {
    /*takes a string of 4-8 characters, 
    and deciphers into information to be sent to the seven seg display
    function*/
    pthread_mutex_lock(&lock);
    SSDon = 1;
    deactivate_segments();
    int disp_dgts[4][8];
    char *num = (char*) no;
    // printf("To 7Seg: %s\n\n", num);
    
    int true_i = 0;
    for (size_t i=0; i<(sizeof &num / sizeof num[0]); i++){
        switch (num[i]){
            case '0':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = zero[j];
                }
                true_i++;
                break;

            case '1':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = one[j];
                }
                true_i++;
                break;

            case '2':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = two[j];
                }
                true_i++;
                break;

            case '3':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = three[j];
                }
                true_i++;
                break;

            case '4':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = four[j];
                }
                true_i++;
                break;

            case '5':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = five[j];
                }
                true_i++;
                break;

            case '6':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = six[j];
                }
                true_i++;
                break;

            case '7':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = seven[j];
                }
                true_i++;
                break;

            case '8':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = eight[j];
                }
                true_i++;
                break;

            case '9':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = nine[j];
                }
                true_i++;
                break;

            case 'E':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = E[j];
                }
                true_i++;
                break;

            case 'r':
                for (int j=0; j<8; j++){
                    disp_dgts[true_i][j] = r[j];
                }
                true_i++;
                break;

            case '.':
                printf("%d", true_i-1);
                disp_dgts[true_i-1][7] = 0;
                break;
        }
    }

    // for (int i=0; i<true_i; i++){
    //     printf("Digit %d: ", i);
    //     for (int j=0; j<8; j++){
    //         printf("%d", disp_dgts[i][j]);
    //     }
    //     printf("\n");
    // }

    while(SSDon){
        clock_t time;
        for (int i=0; i<true_i; i++){
            gpioWrite(dig_list[i], 1);
            for (int j=0; j<8; j++){
                gpioWrite(seg_list[j], disp_dgts[i][j]);
                // printf("%d", disp_dgts[i][j]);
            }
            time = clock();
            while((clock()-time)< 5000);
            // printf(" ");
            gpioWrite(dig_list[i], 0);
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *SS_print2(void* _) {
    /*takes a string of 4-8 characters, 
    and deciphers into information to be sent to the seven seg display
    function*/
    deactivate_segments();
    SSDon = 1;
    int disp_dgts[4][8];
    clock_t time;
    char num[9];
    
    while(SSDon){
        int true_i = 0;
        pthread_mutex_lock(&lock);
        strncpy(num, SSD_disp, 9);
        pthread_mutex_unlock(&lock);
        for (size_t i=0; i<(sizeof &num / sizeof num[0]); i++){
            switch (num[i]){
                case '0':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = zero[j];
                    }
                    true_i++;
                    break;

                case '1':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = one[j];
                    }
                    true_i++;
                    break;

                case '2':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = two[j];
                    }
                    true_i++;
                    break;

                case '3':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = three[j];
                    }
                    true_i++;
                    break;

                case '4':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = four[j];
                    }
                    true_i++;
                    break;

                case '5':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = five[j];
                    }
                    true_i++;
                    break;

                case '6':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = six[j];
                    }
                    true_i++;
                    break;

                case '7':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = seven[j];
                    }
                    true_i++;
                    break;

                case '8':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = eight[j];
                    }
                    true_i++;
                    break;

                case '9':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = nine[j];
                    }
                    true_i++;
                    break;

                case 'E':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = E[j];
                    }
                    true_i++;
                    break;

                case 'r':
                    for (int j=0; j<8; j++){
                        disp_dgts[true_i][j] = r[j];
                    }
                    true_i++;
                    break;

                case '.':
                    printf("%d", true_i-1);
                    disp_dgts[true_i-1][7] = 0;
                    break;
            }
        }

        // for (int i=0; i<true_i; i++){
        //     printf("Digit %d: ", i);
        //     for (int j=0; j<8; j++){
        //         printf("%d", disp_dgts[i][j]);
        //     }
        //     printf("\n");
        // }

        
        for (int i=0; i<true_i; i++){
            gpioWrite(dig_list[i], 1);
            for (int j=0; j<8; j++){
                gpioWrite(seg_list[j], disp_dgts[i][j]);
                // printf("%d", disp_dgts[i][j]);
            }
            time = clock();
            while((clock()-time)< 5000);
            // printf(" ");
            gpioWrite(dig_list[i], 0);
        }
        
    }
    return NULL;
}

/**
 * Returns 1 if given pin reads high for a long period, 0 otherwise.
 * Takes the average of 50 samples to prevent false positives.
 */
int presence_detect(int sensor_pin){
    float tot = 0;
    float sample = 75;
    float avg;
    float hist_avg = 0;
    int j = 1;
    while (j <= 4) {
        tot = 0;
        avg = 0;
        for (int i=0; i<sample; i++) {
            tot += gpioRead(sensor_pin);
        }
        avg = tot/sample;

        if (avg >= 0.98) {
            return 1;
        } else if (avg <=0.02) {
            return 0;
        } else {
            hist_avg += avg; 
        }
        j++;
    }
    hist_avg = hist_avg/j;
    if (hist_avg >= 0.5) {
        return 1;
    } else {
        return 0;
    }
}

/* Smoothly actuates servo to the open position */
void open_door() {
    if (gpioGetServoPulsewidth(Doorservo) < OPEN){
        for (int i=gpioGetServoPulsewidth(Doorservo); i<=OPEN; i+=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%u\n", (unsigned) i);
            gpioDelay(4000);
        }
    } else if (gpioGetServoPulsewidth(Doorservo) > OPEN) {
        for (int i=gpioGetServoPulsewidth(Doorservo); i>=OPEN; i-=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%u\n", (unsigned) i);
            gpioDelay(4000);
        }     
    } else {
        printf("ERROR: door already open");
    }
}

/* Smoothly actuates servo to the close position */
void close_door() {
    if (gpioGetServoPulsewidth(Doorservo) < CLOSE){
        for (int i=gpioGetServoPulsewidth(Doorservo); i<=CLOSE; i+=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%d\n", i);
            gpioDelay(4000);
        }
    } else if (gpioGetServoPulsewidth(Doorservo) > OPEN) {
        for (int i=gpioGetServoPulsewidth(Doorservo); i>=CLOSE; i-=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%d\n", i);
            gpioDelay(4000);
        }     
    } else {
        printf("ERROR: door already closed");
    }
}

/* Switches stepper pin to opposite value -> performs 1 step */
void step(void) {
	if (tick++ % 2)
		gpioWrite(STEP_PIN, 1);
	else
		gpioWrite(STEP_PIN, 0);
	gpioDelay(step_delay_us);
    // printf("stepped\n");
}

/* Performs one vibration oscillation - helper function for vibe_til_drop func */
void vibrate2(void) {
	if (tick++ % 2) {
		gpioWrite(DIR_PIN, 1);
	} else {
		gpioWrite(DIR_PIN, 0);
	}
	gpioWrite(STEP_PIN, 0);
	gpioWrite(STEP_PIN, 1);
	gpioDelay(step_delay_us);
	gpioWrite(STEP_PIN, 0);
	gpioDelay(step_delay_us);
	gpioWrite(STEP_PIN, 1);
	
	// gpioDelay(step_delay_us);
	// gpioWrite(STEP_PIN, 0);
	// gpioDelay(step_delay_us);
	// gpioWrite(STEP_PIN, 1);
	gpioDelay(vibrate_delay_us);
}

/* Steps for step_to_turn times to complete one segment turn. */
void turn(void) {
	gpioWrite(DIR_PIN, TURN_DIRECTION);
    // printf("%d", step_to_turn);
	for (int i = 0; i < step_to_turn; i++) {
        // printf("ree");
		step();
	}
    printf("turned\n");
}

/**
 * Vibrates until the mask falls down. 
 * if timer expires, error code is displayed, but vibration continues
 */
void vibe_til_drop(char stock[8]){
    int start = gpioTick(); // Start mask drop timer
    // After half a sec, vibe until mask drops
    int err0 = 0;
    int count = 0;
    while (presence_detect(IR1) || (count % 2)) {
        count++;
        if (err0) vibrate2();
        // If timer expires, display error to ssd and terminal
        if (((gpioTick() - start) > 2000000) && !err0) {
            printf("ERR0: Mask jammed in magazine.\n");
            update_disp("Err0");
            err0 = 1;
        }
    }
    if (err0) {
        printf("ERR0 cleared: ");
        update_disp(stock);
    }
}

/* Helper function for creating SSD or cmd centre threads */
long unsigned int run_thread(int mode, char num[]){
    if (mode == 0){
        pthread_t thread_id;
        // (ptr to thread id, specific attributes, function to execute on thread, 
        // pass var into func)
        pthread_create(&thread_id, NULL, SS_print, num); 
        return thread_id;
    } else if (mode == 1) {
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, SS_print2, NULL);
        // pthread_detach(thread_id); 
        return thread_id;
    } else {
        printf("invalid mode\n");
    }
    return -1;
}

/**
 * Finds the state corresponding to the given inputs
 */
int find_state(int* INPUTS){
    // Define each state
    // INPUTS = {HAND, IR1, IR2, DOOR}
    static const int ST_1[] = {0, 0, 0, 0}; //waiting for mask
    static const int ST_2[] = {1, 0, 0, 0}; //drop mask
    static const int ST_3[] = {0, 0, 1, 0}; //open door
    static const int ST_4[] = {0, 0, 1, 1}; //feed mask
    static const int ST_5[] = {0, 1, 0, 1}; //take mask
    static const int ST_6[] = {0, 1, 1, 1}; //close door/restock/cleanup
    int ST1 = 0; int ST2 = 0; int ST3 = 0; int ST4 = 0; int ST5 = 0; int ST6 = 0;
    for (int i=0; i<4; i++){
        if (i == 0){
            // the first digit doesn't matter for ST3-6
            ST1 += (ST_1[i] == INPUTS[i]);
            ST2 += (ST_2[i] == INPUTS[i]);
            ST3 += 1;
            ST4 += 1;
            ST5 += 1;
            ST6 += 1;
        } else if (i == 2){
            // the third digit doesn't matter for ST4
            ST1 += (ST_1[i] == INPUTS[i]);
            ST2 += (ST_2[i] == INPUTS[i]);
            ST3 += (ST_3[i] == INPUTS[i]);
            ST4 += 1;
            ST5 += (ST_5[i] == INPUTS[i]);
            ST6 += (ST_6[i] == INPUTS[i]);
        }
        else{
            ST1 += (ST_1[i] == INPUTS[i]);
            ST2 += (ST_2[i] == INPUTS[i]);
            ST3 += (ST_3[i] == INPUTS[i]);
            ST4 += (ST_4[i] == INPUTS[i]);
            ST5 += (ST_5[i] == INPUTS[i]);
            ST6 += (ST_6[i] == INPUTS[i]);
        }
    }

    int statelist[] = {ST1, ST2, ST3, ST4, ST5, ST6};
    int max = 0;
    int maxindex;
    for (int i=0; i<6; i++) {
        int state = statelist[i];
        if (state > max){
            max = state;
            maxindex = i;
        }
    }
    
    // printf("INPUTS: [");
    //     for (int i = 0; i < 4; i++){
    //         printf(" %d", INPUTS[i]);
    //     }
    //     printf(" ]\n ");

    if (max != 4){
        printf("Undefined state: [");
        for (int i = 0; i < 4; i++){
            printf(" %d", INPUTS[i]);
        }
        printf(" ]\n");
        return -1;
    } else {
        // printf("INPUTS: [");
        // for (int i = 0; i < 4; i++){
        //     printf(" %d", INPUTS[i]);
        // }
        // printf(" ], ");
        return maxindex + 1;
    }
}

/**
 * Switches on feed motor until mask is positioned correctly.
 * if mask becomes jammed, Err1 is displayed.
 */
void feed_til_fed(char stock[9]) {
    // gpioWrite(RollMot, 1);  // Switch on feed rollers
    int start = gpioTick(); // Start feed timer
    int IR1_val = 0;
    int IR2_val = 1;

    int err1 = 0;
    while (!IR1_val || IR2_val) {
        // printf("IR1: %d, IR2: %d\n", presence_detect(IR1), presence_detect(IR2));
        // If timer expires, display error to ssd and terminal
        if (((gpioTick() - start) > 5000000) && !err1) {
            printf("ERR1: Mask jammed at feed mechanism\n");
            update_disp("Err1");
            err1 = 1;
        }
        gpioWrite(RollMot, 0);
        gpioDelay(200000);
        IR1_val = presence_detect(IR1);
        IR2_val = presence_detect(IR2);
        gpioWrite(RollMot, 1);
        gpioDelay(100000);
    //     printf("IR1: %d, IR2: %d\n", presence_detect(IR1), presence_detect(IR2));
    }
    if (err1) {
        printf("ERR1 cleared: \n");
        update_disp(stock);        
    }
}

/**
 * Waits for custimer to take mask. If timer expires, mask jammed at door:
 * Err2 is displayed.
 */
void wait_for_take(char stock[9]){
    int start = gpioTick(); // Start take timer

    int err2 = 0;
    while (!gpioRead(IR2)) {
        // If timer expires, display error to ssd and terminal
        if (((gpioTick() - start) > 5000000) && !err2) {
            printf("ERR2: Mask not taken or jammed at door.\n");
            update_disp("Err2");
            err2 = 1;
        }
    }
    if (err2) {
        printf("ERR2 cleared: \n");
        update_disp(stock);
    }
}

/**
 * Decides if a restock is required, if so, the user is asked to quit,
 * or enter the amount of masks restocked
 */
int restock_or_quit(char stock[9]){
    /* TODO: add code here to return stepper to home(?) */
    // Wait to be restocked
    printf("Stock depleted. Please refill\n");
    printf("Enter number of masks restocked, or F to exit process: ");
    scanf("%s", stock);
    int valid_input = 0;
    while (!valid_input){
        if (strchr(stock, 'F')){
            printf("\nexiting...");
            return 0;
        } else if (!(atoi(stock)>=1 && atoi(stock)<=19)){
            printf("\nInvalid input, please try again: ");
            scanf("%s", stock);
        } else {
            //detach and home:
            detach_stepper();
            // home_stepper();
            valid_input = 1;
        }
    }
    return atoi(stock);
}

/* Safely updates SSD using mutex lock */
void update_disp(char stock[9]){
    pthread_mutex_lock(&lock);
    strncpy(SSD_disp, stock, 9);
    pthread_mutex_unlock(&lock);
}

/**
 * Used to safely terminate threads and pigpio - catches SIGINT
 */
void safe_terminate(int dummy) {
    if (dummy != 0) {
        printf("SIGINT detected, ");
    }
    printf("finished, terminating...\n");
    // Ensure loop is exited
    running2 = 0;
    // ensure SSD is off
    SSDon = 0;
    // turn off LEDs
    gpioWrite(LEDs, 0);
    // ensure motors are off
    gpioWrite(RollMot, 0);

    pthread_join(t_id_SSD, NULL);
    printf("Threads joined\n");

    pthread_mutex_destroy(&lock);
    printf("mutex destroyed\n");

    gpioTerminate();
    printf("pigpio terminated\n");
    //exit
    _Exit(0);
}

/* Actuates stepper to the home position */ 
void home_stepper(void){
    printf("Homing...\n");
    // homing slowly
    while(!gpioRead(HOME_RD)) {
        step();
        gpioDelay(7500);
    }
    step();
    printf("Homed\n");
}

/**
 * Detaches the stepper, so it can spin freely, then waits for the user
 * to press enter to reattach the stepper
 */
void detach_stepper(void) {
    gpioWrite(STEP_SLP, 0); // detach stepper
    // wait for user to signal that they are done:
    printf("Stepper power detached, press enter to continue\n");
    getchar();
    getchar();
    gpioWrite(STEP_SLP, 1); // Reattach stepper
    printf("Stepper power reattached\n");
}

/**
 * Sets pin modes, initial pin states/positions,
 * and initialises important variables for the main code
 */
void main_setup(void){
    gpioCfgSetInternals(1<<10);
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
    // open_door();
    // close_door();

    // Initialise multithreading
    SSDon = 0;
    pthread_mutex_init(&lock, NULL);
    
    // initialise stepper vars
    tick = 0;
	step_to_turn = (unsigned)(TOTAL_STEP / SEGMENTS * 2);
	step_delay_us = STEP_DELAY_MS * 1000;
	vibrate_delay_us = VIBRATE_DELAY_MS * 1000;
}

/**
 * Sets pin modes, initial pin states/positions,
 * and initialises important variables for the test code
 */
void setup(void){
    gpioCfgSetInternals(1<<10);
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

    // Initialise multithreading
    SSDon = 0;
    pthread_mutex_init(&lock, NULL);
    
    // initialise stepper vars
    tick = 0;
	step_to_turn = (unsigned)(TOTAL_STEP / SEGMENTS * 2);
	step_delay_us = STEP_DELAY_MS * 1000;
	vibrate_delay_us = VIBRATE_DELAY_MS * 1000;
}

/**
 * Cmd centre function - takes in commands as an integer:
 * 0 - reset 
 * 1 - terminate
 * TBC
 */
// void *command_centre(void *dummy){
//     pthread_mutex_lock(&lock2);
//     int cmd;
//     printf("Welcome to command centre. please enter your command");
//     scanf("%d", &cmd);
//     switch(cmd) {
//         case 0:
//             printf("\n Reset command sent \n");
//             break;
//     }
//     pthread_mutex_unlock(&lock2);
//     return NULL;
// }

// void step_mag(int steps, int direction) {
//     gpioWrite(DirStep, direction);
//     for (int i=0; i<steps; i++) {
//         clock_t time = clock();
//         while ((clock()-time)< spin_delay) {}
//         int curr_level = gpioRead(StepMot);
//         // printf("%d\n", curr_level);
//         if (curr_level == 1) {
//             gpioWrite(StepMot, 0);
//         } else if (curr_level == 0) {
//             gpioWrite(StepMot, 1);
//         } else {
//             printf("Error, bad StepMot GPIO");
//         }
//     }
// }