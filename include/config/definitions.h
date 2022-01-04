#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>

// FILE NAMES
/***********************************/

const char TESTFILE[9] = "testfile";



// PINOUTS
/***********************************/

/*  LOADCELL 1      */
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

/*  SD              */
const int SD_BEGIN_PIN = 14;

// VARIABLES
/***********************************/

/*  GLOBAL          */
const int TIMEOUT = 1000;
const int TIMER = 1000;

/*  SD              */
const int INIT_DELAY = 1000;

/*  TIMER0          */


#endif // DEFINITIONS_H
