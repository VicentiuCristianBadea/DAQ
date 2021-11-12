#ifndef MAIN_CPP
#define MAIN_CPP
#include <Arduino.h>
#include <string.h>
#include "load_cell.cpp"
#include "sd_card.cpp"

// Define pinouts and constant
#include "definitions.cpp"


using namespace std;

LoadCell loadCell_1;
SD_card sd;

void setupSensors();
void setupSDCard();


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  setupSensors();
  setupSDCard();  
}

void loop()
{
  // put your main code here, to run repeatedly:
  setupSensors();
  String line = loadCell_1.readLoadString();
  sd.writeSD("testfile", line);
}


// setup functions
void setupSensors(){
  loadCell_1 = LoadCell();
  loadCell_1.setupLoadCell(LOADCELL_DOUT_PIN, 
                              LOADCELL_SCK_PIN, 
                              TIMEOUT, 
                              TIMER);   
}

void setupSDCard(){
  sd = SD_card();
  sd.setupSD();
}

#endif