#ifndef MAIN_CPP
#define MAIN_CPP
#include <Arduino.h>
#include <string.h>
#include "load_cell.cpp"
#include "sd_card.cpp"

// Define pins
#define onboard 13

using namespace std;

LoadCell loadCell_1 = LoadCell(onboard, OUTPUT);
SD_card sd = SD_card();


int count = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  loadCell_1.blinkLED();
}

#endif