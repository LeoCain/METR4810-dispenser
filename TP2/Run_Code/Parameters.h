/* Header file used to define all parameters which are used in the
Dispensor source code */
#ifndef PARAMETERS
#define PARAMETERS

#include "pinout.h"

#define hand_val 1  //value of HAND sensor when hand is detected

#define step_CW 1 // Value of direction pin to spin stepper CW
#define step_CCW 0 // Value of direction pin to spin stepper CCW
#define next_mask 10 // number of steps required to drop the next mask

// // values of different numbers/characters for seven seg display
// char zero[] = {0, 0, 0, 1, 0, 0, 0, 1};
// char one[] = {1, 1, 0, 1, 0, 1, 1, 1};
// char two[] = {0, 0, 1, 0, 0, 1, 0, 1};
// char three[] = {1, 0, 0, 0, 0, 1, 0, 1};
// char four[] = {1, 1, 0, 0, 0, 0, 1, 1};
// char five[] = {1, 0, 0, 0, 1, 0, 0, 1};
// char six[] = {0, 0, 0, 0, 1, 0, 0, 1};
// char seven[] = {1, 1, 0, 1, 0, 1, 0, 1};
// char eight[] = {0, 0, 0, 0, 0, 0, 0, 1};
// char nine[] = {1, 0, 0, 0, 0, 0, 0, 1};
// char E[] = {0, 0, 1, 0, 1, 0, 0, 1};
// char r[] = {0, 1, 1, 0, 1, 1, 1, 1};

// // Lists containing the seven seg pins in correct order
// int seg_list[] = {SevSegE, SevSegD, SevSegC, SevSegG, SevSegB, SevSegF, SevSegA, SevSegDP};
// int dig_list[] = {SevSegD1, SevSegD2, SevSegD3, SevSegD4};



// #define blank[] = {1, 1, 1, 1, 1, 1, 1, 1}
// #define zero[] = {0, 0, 0, 1, 0, 0, 0, 1}
// #define one[] = {1, 1, 0, 1, 0, 1, 1, 1}
// #define two[] = {0, 0, 1, 0, 0, 1, 0, 1}
// #define three[] = {1, 0, 0, 0, 0, 1, 0, 1}
// #define four[] = {1, 1, 0, 0, 0, 0, 1, 1}
// #define five[] = {1, 0, 0, 0, 1, 0, 0, 1}
// #define six[] = {0, 0, 0, 0, 1, 0, 0, 1}
// #define seven[] = {1, 1, 0, 1, 0, 1, 0, 1}
// #define eight[] = {0, 0, 0, 0, 0, 0, 0, 1}
// #define nine[] = {1, 0, 0, 0, 0, 0, 0, 1}
// #define E[] = {0, 0, 1, 0, 1, 0, 0, 1}
// #define r[] = {0, 1, 1, 0, 1, 1, 1, 1}

// // Lists containing the seven seg pins in correct order
// #define seg_list[] = {SevSegE, SevSegD, SevSegC, SevSegG, SevSegB, SevSegF, SevSegA, SevSegDP}
// #define dig_list[] = {SevSegD1, SevSegD2, SevSegD3, SevSegD4}

// #define hand_present 1  //value of HAND sensor when hand is detected

#endif

