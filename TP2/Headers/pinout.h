// header file to describe the function of Rpi pins
#ifndef PINOUT
#define PINOUT

// Seven seg pins
#define SevSegA 27 // segment A of seven seg
#define SevSegB 14 // segment B of seven seg
#define SevSegC 11 // segment C of seven seg
#define SevSegD 5 // segment D of seven seg
#define SevSegE 6 // segment E of seven seg 
#define SevSegF 22 // segment F of seven seg
#define SevSegG 9
#define SevSegDP 0 // decimal point
#define SevSegD1 17 // First digit
#define SevSegD2 18 // second digit
#define SevSegD3 15 // third digit
#define SevSegD4 10 // 4th digit   

#define RollMot 19 // Roller DC motor

// Stepper motor pins
#define STEP_PIN 20 // Stepper motor step
#define DIR_PIN 21 // stepper motor direction
#define STEP_SLP 16 // detach stepper motor power 1 = not sleep, 0 = sleep
#define HOME_RD 2 // HIGH when stepper is in home position

// IR Phototransistor pins
#define IR1 24
#define IR2 23

#define HAND 26 // hand detection sensor

#define Doorservo 12 // servo for sliding door [PWM]

#define LEDs 13 // Multiple IR LEDs and green LED[PWM]

#endif
