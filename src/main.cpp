#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Wire.h>

#include "load_cell.h"
#include "sd_card.h"
#include "data.h"

#include "config/definitions.h"


using namespace std;

LoadCell loadCell_1;
SD_card sd;
Data data;

void setupSensors();
void setupSDCard();
void setupData();



void setup()
{
  
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);


  setupSensors();
  setupSDCard();  
  setupData();
}

void loop()
{
  Serial.println("Hello World");
  delay(1000);
  digitalWrite(PC13, HIGH);
  sd.writeSD(TESTFILE, loadCell_1.readLoadString(data));
  delay(1000);
  digitalWrite(PC13, LOW);
  // Do nothing
}


// setup functions
void setupSensors(){
  loadCell_1 = LoadCell();
  loadCell_1.setupLoadCell(LOADCELL_DOUT_PIN, 
                              LOADCELL_SCK_PIN, 
                              TIMEOUT, 
                              TIMER);   
  loadCell_1.loadCellBegin();
}

void setupSDCard(){
  sd = SD_card();
  sd.setupSD();
}

void setupData(){
  data = Data();
}

// ISR(TIMER1_COMPA_vect){
//   // String data1 = loadCell_1.readLoadString(data, TIMER);
//   // sd.writeSD(TESTFILE, data1);
//   Serial.println(millis());
// }


// void unitTestFile(){
//   delay(10)
//   String fakeData = random(10).toString();
//   sd.writeSD(TESTFILE, fakeData);
// }

