#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>

// FILE NAMES
/***********************************/

const char TESTFILE[9] = "testfile";



// PINOUTS
/***********************************/


// #define LOADCELL_DOUT_PIN_1     PA0
// #define LOADCELL_DOUT_PIN_2     PA1
#define LOADCELL_SCK_PIN        PB10

#define SD_BEGIN_PIN            PA4
#define HALL_EFFECT_1   PA4 
#define HALL_EFFECT_2   PA5


// VARIABLES
/***********************************/

#define TIMEOUT             1000
#define TIMER               1000

// SD
#define INIT_DELAY          1000

// LOADCELLS
#define READ_DATA_TIMER_FREQ 100
#define CALIBRATION_FACTOR -7050
#define READ_DATA_TIMER TIM4

// LINEAR POTS
#define MAX_LINEAR_POT_DELTA 600
const int l1_pin = PA0;
const int l2_pin = PA1;

// DRIVER INPUT
const int di_pin = PB1;

// MOTOR 1
const int m1_left = PB3;
const int m1_right = PB4;
const int m1_motorPin = PA15;
const int m1_encoder_A = PA2;
const int m1_encoder_B = PA3;

const int motorSpeed = 255;
#define turnClockWise 1
#define turnCounterClockWise -1
#define stopRotating 0

// MOTOR 2
const int m2_left = PB15;
const int m2_right = PB13;
const int m2_motorPin = PB14;
const int m2_encoder_A = PB0;
const int m2_encoder_B = PB2;

// PID 
#define kp 15  //  Try changing kp to 5 to reduce jitter
#define kd 1
#define ki 0.2   //  Try changing ki to 10 to provide more power

#define PID_MAXPOWER 255

#endif // DEFINITIONS_H
