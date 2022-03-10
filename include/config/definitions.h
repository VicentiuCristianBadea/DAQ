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


// VARIABLES
/***********************************/

#define TIMEOUT             1000
#define TIMER               1000

// SD
#define INIT_DELAY          1000


// LOADCELLS
#define LOADCELL_TIMER_FREQ 100
#define CALIBRATION_FACTOR -7050
#define LOADCELL_TIMER TIM4

// MOTOR 1
#define m1_left PB3
#define m1_right PB4
#define m1_motorPin PA15
#define motorSpeed 255
#define m1_encoder_A PA2
#define m1_encoder_B PA3

#define turnClockWise 1
#define turnCounterClockWise -1
#define stopRotating 0

// MOTOR 2
#define m2_left PB15
#define m2_right PB13
#define m2_motorPin PB14
#define m2_encoder_A PB0
#define m2_encoder_B PB2

// PID 
#define kp 1.5
#define kd 0.05
#define ki 0

#endif // DEFINITIONS_H
