/*
Test code for the 5461BS Seven Seg Display.
*/
#include "../Headers/pinout.h"
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int main() {
    setup();
    signal(SIGINT, safe_terminate);

    pthread_mutex_lock(&lock);
    strncpy(SSD_disp, "4810", 9);
    pthread_mutex_unlock(&lock);

    t_id_SSD = run_thread(1, " ");
    sleep(3);
    pthread_mutex_lock(&lock);
    strncpy(SSD_disp, "Err0", 9);
    pthread_mutex_unlock(&lock);
    sleep(3);
    SSDon = 0;
    
    gpioTerminate();
}