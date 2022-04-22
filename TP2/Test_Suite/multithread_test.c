#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include "../Run_Code/pinout.h"
#include "../Run_Code/Parameters.h"
#include <pigpio.h>

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

int seg_list[] = {SevSegE, SevSegD, SevSegC, SevSegG, SevSegB, SevSegF, SevSegA, SevSegDP};
int dig_list[] = {SevSegD1, SevSegD2, SevSegD3, SevSegD4};

pthread_mutex_t lock;
int SSDon;

void *display_sender(void *no) {
    /*takes a string of 4 characters, 
    and deciphers into information to be sent to the seven seg display
    function*/
    pthread_mutex_lock(&lock);
    SSDon = 1;
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
            while((clock()-time)< 5000){}
            // printf(" ");
            gpioWrite(dig_list[i], 0);
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void setup(){
    // num[0] = "0000";
    gpioInitialise();
    SSDon = 0;
    pthread_mutex_init(&lock, NULL);
}
int main(){
    setup();
    pthread_t t_id;    // Identify the thread
    printf("printing Err0...\n");

    pthread_create(&t_id, NULL, display_sender, "10");    // (ptr to thread id, specific attributes, 
                                                            // function to execute on thread, pass var into func)
    printf("SSD updated\n");
    sleep(4);
    printf("turning off SSD, printing Err1\n");
    SSDon = 0;
    pthread_join(t_id, NULL);

    pthread_create(&t_id, NULL, display_sender, "Err1");
    sleep(4);
    SSDon = 0;
    pthread_join(t_id, NULL);
    printf("SSD is off\n");
    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);
    gpioTerminate();
}