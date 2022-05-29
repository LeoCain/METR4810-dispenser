/* Header file used to define all parameters which are used in the
Dispensor source code */
#ifndef PARAMETERS
#define PARAMETERS

#include "pinout.h"
#include <pthread.h>

/* Mask request detect sensor */
#define hand_val            1  //value of HAND sensor when hand is detected

/* IR break sensors */
#define LED_BRIGHTNESS      245

/* Stepper */
#define TOTAL_STEP		    200
#define STEP_DELAY_MS		20
#define VIBRATE_DELAY_MS	20
#define VIBRATE_STEPS		2 // minimum 2
#define SEGMENTS		    20  // Number of segments in the magazine


/* Stepper Demo (for test code)*/
#define DEMO_TURNS		    5
#define DEMO_VIBRATE		52  //must be even
#define TURN_DIRECTION		1

/* System */
#define UNDEF_STATE         0
#define WAIT                1
#define DROP_MASK           2
#define OPEN_DOOR           3
#define FEED_MASK           4
#define TAKE_MASK           5
#define FINISH              6  

/* Servo */
#define MAX                 2500   // Maximum safe pwm value for servo
#define MIN                 500    // Minimum safe pwm value for servo
#define OPEN                950   // Value of pwm for servo OPEN position
#define CLOSE               2200 // Value of pwm for servo CLOSE position

/* initialise global vars */
int SSDon;
char SSD_disp[9];
static volatile int running2 = 1;
long unsigned int t_id_SSD;
pthread_mutex_t lock;

#endif

