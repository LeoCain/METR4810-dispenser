#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include "../Headers/Dispenser_lib.h"
#include "../Headers/Parameters.h"
#include "../Headers/pinout.h"

/**
 * Demos the turn and vibrate functionality.
 */
void demo(void)
{
	printf("Demo TURN\n");
	for (int n = 0; n < DEMO_TURNS; n++) {
		printf("Turn: %d\n", n);
		turn();
	}

	gpioDelay(10000);

	printf("Demo VIBRATE\n");
	for (int n = 0; n < DEMO_VIBRATE; n++) {
		if (n % VIBRATE_DELAY_MS)
			printf("Vibrate: %d\n", n);
		vibrate2();
	}

	printf("Done\n");
}

/**
 * Initialise GPIO pins and global variables.
 */
void init(void)
{
	gpioSetMode(STEP_PIN, PI_OUTPUT);
	gpioSetMode(DIR_PIN, PI_OUTPUT);
	gpioSetMode(STEP_SLP, PI_OUTPUT);
	gpioWrite(DIR_PIN, TURN_DIRECTION);
	gpioWrite(STEP_SLP, 1);
	tick = 0;
	step_to_turn = (unsigned)(TOTAL_STEP / SEGMENTS * 2);
	step_delay_us = STEP_DELAY_MS * 1000;
	vibrate_delay_us = VIBRATE_DELAY_MS * 1000;
}

int main(void)
{
	gpioInitialise();
	init();
    demo();
	return 0;
}