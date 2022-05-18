/* Header file used to define all parameters which are used in the
Dispensor source code */
#ifndef PARAMETERS
#define PARAMETERS

#include "pinout.h"

#define hand_val 1  //value of HAND sensor when hand is detected

// #define step_CW 1 // Value of direction pin to spin stepper CW
// #define step_CCW 0 // Value of direction pin to spin stepper CCW
// #define next_mask 10 // number of steps required to drop the next mask
// #define spin_delay 75000 // delay (in ticks) of pause between each step
/* Stepper */
#define TOTAL_STEP			200
#define STEP_DELAY_MS		20
#define VIBRATE_DELAY_MS	10
#define VIBRATE_STEPS		2 // minimum 2
/* System */
#define SEGMENTS			20
/* Demo */
#define DEMO_TURNS			1
#define DEMO_VIBRATE		52 //must be even
#define TURN_DIRECTION		1

#define MAX 2500 
#define MIN 500
#define OPEN 1170   // Value of pwm for servo OPEN position
#define CLOSE 1710  // Value of pwm for servo CLOSE position

#endif

