/*
Test code for the 5461BS Seven Seg Display.
*/
#include "../Run_Code/pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char blank[] = {1, 1, 1, 1, 1, 1, 1, 1};
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

// define sev seg display pins
int seg_list[] = {SevSegE, SevSegD, SevSegC, SevSegG, SevSegB, SevSegF, SevSegA, SevSegDP};
int dig_list[] = {SevSegD1, SevSegD2, SevSegD3, SevSegD4};

void display_sender(char num[]) {
    /*takes a string of 4 characters, 
    and deciphers into information to be sent to the seven seg display
    function*/
    int disp_dgts[4][8];
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

    while(1){
        clock_t time;
        for (int i=0; i<true_i; i++){
            gpioWrite(dig_list[i], 1);
            for (int j=0; j<8; j++){
                gpioWrite(seg_list[j], disp_dgts[i][j]);
                // printf("%d", disp_dgts[i][j]);
            }
            time = clock();
            while((clock()-time)< 5000){}
            // printf(" ");
            gpioWrite(dig_list[i], 0);
        }
    }
}

// Test code for seven seg
int main(){
	gpioInitialise();
	for (int i=0; i<8; i++){
        gpioSetMode(seg_list[i], PI_OUTPUT);
        gpioWrite(seg_list[i], 1); // deactivate all segs
    }
    for (int i=0; i<4; i++){
        gpioSetMode(dig_list[i], PI_OUTPUT);
        gpioWrite(dig_list[i], 0); //deactivate all digits
    }
	char num[] = "4810";
    display_sender(num);
    return 1;
}
