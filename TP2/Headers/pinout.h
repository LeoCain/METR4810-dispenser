// header file to describe the function of Rpi pins
#ifndef PINOUT
#define PINOUT

// Seven seg pins
#define SevSegA 9 // segment A of seven seg
#define SevSegB 17 // segment B of seven seg
#define SevSegC 2 // segment C of seven seg
#define SevSegD 5 // segment D of seven seg
#define SevSegE 0 // segment E of seven seg 
#define SevSegF 10 // segment F of seven seg
#define SevSegG 3 
#define SevSegDP 6 // decimal point
#define SevSegD1 11 // First digit
#define SevSegD2 22 // second digit
#define SevSegD3 27 // third digit
#define SevSegD4 4 // 4th digit   

#define RollMot 18 // Roller DC motor

// Stepper motor pins
#define STEP_PIN 14 // Stepper motor step
#define DIR_PIN 15 // stepper motor direction
#define STEP_SLP 8 // detach stepper motor power 1 = not sleep, 0 = sleep
#define HOME 19 // HIGH when stepper is in home position

// IR Phototransistor pins
#define IR1 24
#define IR2 23

#define HAND 25 // hand detection sensor

#define Doorservo 12 // servo for sliding door [PWM]

#define IRLED 13 // Multiple IR LEDs [PWM]

#endif