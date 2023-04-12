#ifndef definitions
#define definitions

//include libaries
#include <Arduino.h>
#include <Wire.h>                   //libary for I2C
#include <LiquidCrystal_I2C.h>      //libary for LCD-I2C
#include <Servo.h>                  //libary for Servo

//create objects to control servos
extern Servo servo1;
extern Servo servo2;
extern Servo servo3;
extern Servo servo4;

extern LiquidCrystal_I2C lcd;

//names and pinnumbers of fixed variables
//button variables
#define button1 22
#define button2 23
#define button3 24
#define button4 25

#define button_res 26
#define button_select 27
#define button_mode_up 28
#define button_mode_down 29
//vibration motor variables
#define vibration1 30
#define vibration2 31
#define vibration3 32
#define vibration4 33
#define vibration5 34
#define vibration6 35
#define vibration7 36
#define vibration8 37
//FSR sensor variable (four on one pin)
#define FSR A0
#define FSR4 A1
#define FSR3 A2
#define FSR2 A3
#define FSR1 A4
//names and pinnumbers of changing variables
//variable for increasing only once if button 1 is pressed and not counting up as long as finger is on button 1
extern int prestate1;
extern int prestate2;
extern int prestate3;
extern int prestate4;
//extern int prestate_mode_up;
//extern int prestate_mode_down;
extern int prestate_select;
extern int prestate_res;
//variable for button-press counter (increases with every buttonpress)
extern int button_press1;
extern int button_press2;
extern int button_press3;
extern int button_press4;

#endif
