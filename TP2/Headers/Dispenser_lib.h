/* Header file used to initialise functions that the
dispensor main will use */

#ifndef DLIB
#define DLIB

#include <pthread.h>
/*** Seven Segment Display ***/
void deactivate_segments(); // turns off all SS LEDs.
void *SS_print(void *no); // old depreciated function, replaced with SS_print2 
void *SS_print2(void* _);
long unsigned int run_thread(int mode, char num[]);
void update_disp(char stock[9]);

/*** Sensor observation, state detection ***/
int presence_detect(int sensor_pin); 
int find_state(int INPUTS[]);

/*** Servo ***/
void open_door(); // Actuates Door Servo to the OPEN position
void close_door(); // Actuates Door Servo to the OPEN position

/*** Stepper ***/
void turn(void);
void vibrate2(void);
void step(void);

/*** Functional helper functions ***/
void vibe_til_drop(char stock[8]);
void feed_til_fed(char stock[9]);
void wait_for_take(char stock[9]);
int restock_or_quit(char stock[8]);

/*** Global variables ***/
// multithreading
extern int SSDon;
extern pthread_mutex_t lock;
// stepper
unsigned tick;
unsigned step_to_turn;
unsigned step_delay_us;
unsigned vibrate_delay_us;

#endif



