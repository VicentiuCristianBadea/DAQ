#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>

// FILE NAMES
/***********************************/

const char TESTFILE[9] = "testfile";



// PINOUTS
/***********************************/


#define LOADCELL_DOUT_PIN_1     PA0
#define LOADCELL_DOUT_PIN_2     PA1
#define LOADCELL_SCK_PIN        PB10

#define SD_BEGIN_PIN            PA4


// VARIABLES
/***********************************/

#define TIMEOUT             1000
#define TIMER               1000

// SD
#define INIT_DELAY          1000

// TIMER4
#define LOADCELL_TIMER_FREQ 100


#endif // DEFINITIONS_H
