#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

/* Pins */
#define STEP_PIN			14
#define DIR_PIN				15
#define STEP_SLP			8
/* Stepper */
#define TOTAL_STEP			200
#define STEP_DELAY_MS		20
#define VIBRATE_DELAY_MS	10
#define VIBRATE_STEPS		2 // minimum 2
/* System */
#define SEGMENTS			20
/* Demo */
#define DEMO_TURNS			100
#define DEMO_VIBRATE		52 //must be even
#define TURN_DIRECTION		1

static unsigned tick;
static unsigned step_to_turn;
static unsigned step_delay_us;
static unsigned vibrate_delay_us;

/**
 * Increment stepper `step` tick and write 1 or 0.
 * Delays for step_delay microseconds.
 */
static inline void step(void)
{
	if (tick++ % 2)
		gpioWrite(STEP_PIN, 1);
	else
		gpioWrite(STEP_PIN, 0);
	gpioDelay(step_delay_us);
}

/**
 * Switches direction and steps for VIBRATE_STEPS.
 * Delays for vibrate_delay microseconds.
 */
void vibrate(void)
{
	if (tick % 2)
		gpioWrite(DIR_PIN, 1);
	else
		gpioWrite(DIR_PIN, 0);
	for (int i = 0; i < VIBRATE_STEPS; i++){
		step();
	}
	// tick++;
	gpioDelay(vibrate_delay_us);
}

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

/**
 * Steps for step_to_turn times to complete one segment turn.
 */
void turn(void)
{
	gpioWrite(DIR_PIN, TURN_DIRECTION);
	for (int i = 0; i < step_to_turn; i++) {
		step();
	}
}

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
	// gpioDelay(10000);
	// printf("Demo VIBRATE\n");
	// for (int n = 0; n < DEMO_VIBRATE; n++) {
	// 	if (n % VIBRATE_DELAY_MS)
	// 		printf("Vibrate: %d\n", n);
	// 	vibrate2();
	// }

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
	turn();
	// demo();
	// while(1){
	// 	printf("unlocked\n");
	// 	gpioWrite(STEP_SLP, 0);
	// 	sleep(5);
	// 	gpioWrite(STEP_SLP, 1);
	// 	printf("locked\n");
	// 	gpioDelay(10000);
	// 	turn();
	// 	sleep(5);
	// }
	return 0;
}

