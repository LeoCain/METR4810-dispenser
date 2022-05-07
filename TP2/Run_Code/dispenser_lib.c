/*
Src file for the dispenser library
*/
#include "Dispenser_lib.h"
#include "Parameters.h"
#include "pinout.h"
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

/**
 * Returns 1 if hand is detected, 0 otherwise.
 * Takes the average of 50 samples to prevent false positives.
 */
int hand_present(){
    float tot = 0;
    int sample = 50;
    float avg;
    for (int i=0; i<sample; i++) {
        tot += gpioRead(HAND);
    }
    avg = tot/sample;

    if (avg == hand_val && avg == gpioRead(HAND)){
        return 1;
    } else{
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
    printf("stepped\n");
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
long unsigned int vibe_til_drop(long unsigned int t_id, char stock[8]){
    int start = gpioTick(); // Start mask drop timer
    // After half a sec, vibe until mask drops
    int err0 = 0;
    while (gpioRead(IR1)) {
        vibrate2();
        // If timer expires, display error to ssd and terminal
        if (((gpioTick() - start) > 2000000) && !err0) {
            printf("ERR0: Mask jammed in magazine.\n");
            SSDon = 0;
            pthread_join(t_id, NULL);
            t_id = run_thread(0, "Err0");
            err0 = 1;
        }
    }
    if (err0) {
        printf("ERR0 cleared: Mask dropped.\n");
        SSDon = 0;
        pthread_join(t_id, NULL);
        t_id = run_thread(0, stock);
    }
    return t_id;
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
        pthread_create(&thread_id, NULL, command_centre, NULL); 
        return thread_id;
    } else {
        printf("invalid mode\n");
    }
    return -1;
}

/**
 * Cmd centre function - takes in commands as an integer:
 * 0 - reset 
 * 1 - terminate
 * TBC
 */
void *command_centre(void *dummy){
    pthread_mutex_lock(&lock2);
    int cmd;
    printf("Welcome to command centre. please enter your command");
    scanf("%d", &cmd);
    switch(cmd) {
        case 0:
            printf("\n Reset command sent \n");
            break;
    }
    pthread_mutex_unlock(&lock2);
    return NULL;
}

/**
 * Finds the state corresponding to the given inputs
 */
int find_state(int* INPUTS){
    // Define each state
    int ST_1[] = {0, 1, 1, 0};
    int ST_2[] = {1, 1, 1, 0};
    int ST_3[] = {0, 0, 1, 0};
    int ST_4[] = {0, 0, 1, 1};
    int ST_5[] = {0, 1, 0, 1};
    int ST_6[] = {0, 1, 1, 1};
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
        } else{
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
    if (max != 4){
        printf("Error, impossible truth: [");
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
long unsigned int feed_til_fed(long unsigned int t_id, char stock[8]){
    gpioWrite(RollMot, 1);  // Switch on feed rollers
    int start = gpioTick(); // Start feed timer

    int err1 = 0;
    while (!gpioRead(IR1)) {
        // If timer expires, display error to ssd and terminal
        if (((gpioTick() - start) > 2500000) && !err1) {
            printf("ERR1: Mask jammed at feed mechanism.\n");
            SSDon = 0;
            pthread_join(t_id, NULL);
            t_id = run_thread(0, "Err1");
            err1 = 1;
        }
    }
    if (err1) {
        printf("ERR1 cleared: Mask fed.\n");
        SSDon = 0;
        pthread_join(t_id, NULL);
        t_id = run_thread(0, stock);
    }
    gpioWrite(RollMot, 0); // Turn off feed motor
    return t_id;
}

/**
 * Waits for custimer to take mask. If timer expires, mask jammed at door:
 * Err2 is displayed.
 */
long unsigned int wait_for_take(long unsigned int t_id, char stock[8]){
    int start = gpioTick(); // Start take timer

    int err2 = 0;
    while (!gpioRead(IR2)) {
        // If timer expires, display error to ssd and terminal
        if (((gpioTick() - start) > 5000000) && !err2) {
            printf("ERR2: Mask not taken or jammed at door.\n");
            SSDon = 0;
            pthread_join(t_id, NULL);
            t_id = run_thread(0, "Err2");
            err2 = 1;
        }
    }
    if (err2) {
        printf("ERR2 cleared: Mask taken.\n");
        SSDon = 0;
        pthread_join(t_id, NULL);
        t_id = run_thread(0, stock);
    }
    return t_id;
}

/**
 * Decides if a restock is required, if so, the user is asked to quit,
 * or enter the amount of masks restocked
 */
int restock_or_quit(long unsigned int t_id_cmd, char stock[8]){
    if (atoi(stock) <= 0) {
        // TODO: add code here to return stepper to home
        // Command centre is using scanf, cancel it's thread safely:
        pthread_cancel(t_id_cmd);
        printf("cmd thread cancelled\n");
        pthread_mutex_unlock(&lock2);
        printf("mutex unlocked\n");
        pthread_join(t_id_cmd, NULL);
        printf("cmd thread joined\n");
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
                valid_input = 1;
            }
        }
        return atoi(stock);
    } else {
        return atoi(stock);
    }
}

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