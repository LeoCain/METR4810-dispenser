/*
Test code for the 5461BS Seven Seg Display.
*/
#include "../Headers/pinout.h"
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    gpioInitialise();
    t_id_SSD = run_thread(0, "4810");
    sleep(3);
    SSDon = 0;
    pthread_join(t_id_SSD, NULL);
    gpioTerminate();
}