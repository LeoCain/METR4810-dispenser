/* Header file used to define all parameters which are used in the
Dispensor source code */
#ifndef PARAMETERS
#define PARAMETERS

#include "pinout.h"

#define hand_val 1  //value of HAND sensor when hand is detected

#define step_CW 1 // Value of direction pin to spin stepper CW
#define step_CCW 0 // Value of direction pin to spin stepper CCW
#define next_mask 10 // number of steps required to drop the next mask

#define OPEN 1000   // Value of pwm for servo OPEN position
#define CLOSE 2000  // Value of pwm for servo CLOSE position

#endif

