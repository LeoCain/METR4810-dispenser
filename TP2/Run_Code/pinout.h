// header file to describe the function of Rpi pinouts
#ifndef PINOUT
#define PINOUT

// Seven seg pins
#define SevSegA 9 // segment A of seven seg
#define SevSegB 6 // segment B of seven seg
#define SevSegC 17 // segment C of seven seg
#define SevSegD 3 // segment D of seven seg
#define SevSegE 2 // segment E of seven seg
#define SevSegF 11 // segment F of seven seg
#define SevSegG 27 
#define SevSegDP 4 // decimal point
#define SevSegD1 10 // First digit
#define SevSegD2 0 // second digit
#define SevSegD3 5 // third digit
#define SevSegD4 22 // 4th digit   

#define RollMot 18 // Roller DC motor

// Stepper motor pins
#define StepMot 14 // Stepper motor step
#define DirStep 15 // stepper motor direction

// IR Phototransistor pins
#define IR1 24
#define IR2 23  

#define HAND 25 // hand detection sensor

#define Doorservo 12 // servo for sliding door [PWM]

#define IRLED 13 // Multiple IR LEDs [PWM]

#endif