#ifndef MAIN_CPP
#define MAIN_CPP
#include <Arduino.h>
#include <string.h>
#include "load_cell.cpp"



// Define pins
#define onboard 13


// Define sensors



using namespace std;


LoadCell loadCell_1 = LoadCell(onboard, OUTPUT);

int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  loadCell_1.blinkLED();
}

#endif