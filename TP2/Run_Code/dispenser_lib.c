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

// multithreading flag and mutex lock

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

void *SS_print(void *no) {
    /*takes a string of 4-8 characters, 
    and deciphers into information to be sent to the seven seg display
    function*/
    pthread_mutex_lock(&lock);
    SSDon = 1;
    deactivate_segments();
    int disp_dgts[4][8];
    char *num = no;
    printf("To 7Seg: %s\n\n", num);
    
    int true_i = 0;
    for (int i=0; i<(sizeof &num / sizeof num[0]); i++){
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

    for (int i=0; i<true_i; i++){
        printf("Digit %d: ", i);
        for (int j=0; j<8; j++){
            printf("%d", disp_dgts[i][j]);
        }
        printf("\n");
    }

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

int hand_present(){
    /* Returns 1 if hand is detected, 0 otherwise */
    float tot = 0;
    int sample = 50;
    float avg;
    for (int i=0; i<sample; i++) {
        tot += gpioRead(HAND);
    }
    avg = tot/sample;
    // if (avg==hand_val) {
    //     tot=0;
    //     for (int i=0; i<10; i++) {
    //         tot += gpioRead(HAND);
    //     }
    //     avg = tot/10;
    // }
    if (avg == hand_val && avg == gpioRead(HAND)){
        return 1;
    } else{
        return 0;
    }
}

void open_door() {
    /* actuates servo to the open position */
    if (gpioGetServoPulsewidth(Doorservo) < OPEN){
        for (int i=gpioGetServoPulsewidth(Doorservo); i<=OPEN; i+=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%u\n", (unsigned) i);
            gpioDelay(3000);
        }
    } else if (gpioGetServoPulsewidth(Doorservo) > OPEN) {
        for (int i=gpioGetServoPulsewidth(Doorservo); i>=OPEN; i-=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%u\n", (unsigned) i);
            gpioDelay(3000);
        }     
    } else {
        printf("ERROR: door already open");
    }
}

void close_door() {
    /* actuates servo to the open position */
    if (gpioGetServoPulsewidth(Doorservo) < CLOSE){
        for (int i=gpioGetServoPulsewidth(Doorservo); i<=CLOSE; i+=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%d\n", i);
            gpioDelay(3000);
        }
    } else if (gpioGetServoPulsewidth(Doorservo) > OPEN) {
        for (int i=gpioGetServoPulsewidth(Doorservo); i>=CLOSE; i-=10) {
            gpioServo(Doorservo, (unsigned) i);
            // printf("%d\n", i);
            gpioDelay(3000);
        }     
    } else {
        printf("ERROR: door already closed");
    }
}

void step_mag(int steps, int direction) {
    gpioWrite(DirStep, direction);
    for (int i=0; i<steps; i++) {
        clock_t time = clock();
        while ((clock()-time)< spin_delay) {}
        int curr_level = gpioRead(StepMot);
        // printf("%d\n", curr_level);
        if (curr_level == 1) {
            gpioWrite(StepMot, 0);
        } else if (curr_level == 0) {
            gpioWrite(StepMot, 1);
        } else {
            printf("Error, bad StepMot GPIO");
        }
    }
}