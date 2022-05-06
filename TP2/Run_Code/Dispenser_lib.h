/* Header file used to initialise functions that the
dispensor main will use */

#ifndef DLIB
#define DLIB

#include <pthread.h>

void deactivate_segments(); // turns off all SS LEDs.
void *SS_print(void *no); // Takes in a string, and prints to Sev Seg display

int hand_present(); // returns 1 if hand is present.

// void step_mag(int steps, int direction); // steps the stepper "steps" times
                                            // in "direction" direction

void open_door(); // Actuates Door Servo to the OPEN position
void close_door(); // Actuates Door Servo to the OPEN position

long unsigned int run_thread(int mode, char num[]);
void *command_centre(void *dummy);

int find_state(int INPUTS[]);

void turn(void);
void vibrate2(void);
void step(void);
long unsigned int vibe_til_drop(long unsigned int t_id, char stock[8]);
long unsigned int feed_til_fed(long unsigned int t_id, char stock[8]);
long unsigned int wait_for_take(long unsigned int t_id, char stock[8]);
long unsigned int restock_or_quit(long unsigned int t_id_cmd, char stock[8]);

extern int SSDon;
extern pthread_mutex_t lock;
extern pthread_mutex_t lock2;
// stepper stuff
unsigned tick;
unsigned step_to_turn;
unsigned step_delay_us;
unsigned vibrate_delay_us;

#endif



