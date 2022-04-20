/* Header file used to initialise functions that the
dispensor main will use */
#ifndef DLIB
#define DLIB

void deactivate_segments(); // turns off all SS LEDs.

void SS_print(char num[]); // Takes in a string, and prints to Sev Seg display

int hand_present(); // returns 1 if hand is present.

void step_mag(int steps, int direction); // steps the stepper "steps" times
                                            // in "direction" direction

void open_door(); // Actuates Door Servo to the OPEN position

void close_door(); // Actuates Door Servo to the OPEN position
#endif



