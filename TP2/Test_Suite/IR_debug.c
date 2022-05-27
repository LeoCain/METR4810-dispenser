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

int main(void) {
    signal(SIGINT, safe_terminate);
    while (running2) {
        printf("IR1: %d, IR2 %d, HAND: %d\n", presence_detect(IR1), presence_detect(IR2), presence_detect(HAND));
    }
    return 0;
}