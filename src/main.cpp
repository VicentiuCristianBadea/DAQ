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
LoadCell loadCell_2;
SD_card sd;
Data data;

int input_freq;

void setupSensors();
void setupSDCard();
void setupData();
void createSensor(LoadCell&, int);
void ledON();
void ledOFF();
void heartBeat();
void interruptReadData();

void setup()
{
  
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);


  setupSensors();
  setupSDCard();  
  setupData();


  TIM_TypeDef* Instance = TIM5;
  HardwareTimer * MyTim = new HardwareTimer(Instance);
 
  MyTim->pause();
  MyTim->refresh();
  MyTim->setOverflow(1000, MICROSEC_FORMAT);
  MyTim->attachInterrupt(interruptReadData);
  MyTim->resume();

  input_freq = MyTim->getTimerClkFreq()/ MyTim->getPrescaleFactor();
}


void loop()
{
  // heartBeat();
}


// setup functions
void setupSensors(){
  createSensor(loadCell_1, LOADCELL_DOUT_PIN_1);
  createSensor(loadCell_2, LOADCELL_DOUT_PIN_2);
}

void createSensor(LoadCell &loadCell, int dout_pin){
  loadCell = LoadCell();
  loadCell.setupLoadCell(dout_pin, LOADCELL_SCK_PIN, TIMEOUT);   
  loadCell.loadCellBegin();
}

void setupSDCard(){
  sd = SD_card();
  sd.setupSD(TESTFILE);
}

void setupData(){
  data = Data();
}

void ledON(){
  digitalWrite(PC13, HIGH);
}

void ledOFF(){
  digitalWrite(PC13, LOW);
}

void heartBeat(){
  ledON();
  delay(100);
  ledOFF();
  delay(100);
  ledON();
  delay(300);
  ledOFF();
}

void interruptReadData(){
  data.emptyData();
  data.concatData(String(millis()));
  data.concatData(loadCell_1.readLoadString());
  data.concatData(loadCell_2.readLoadString());
  Serial.println(String(millis()));
  Serial.println(input_freq);
  // sd.writeSD(data.getData());
}

// ISR(TIMER1_COMPA_vect){
//   // String data1 = loadCell_1.readLoadString(data, TIMER);
//   // sd.writeSD(TESTFILE, data1);
//   Serial.println(millis());
// }


// void TIM4_IRQHandler(){

//   Serial.println("This is the interrupt firing");
//   TIM4->SR &= ~TIM_SR_UIF;
// }

// void unitTestFile(){
//   delay(10)
//   String fakeData = random(10).toString();
//   sd.writeSD(TESTFILE, fakeData);
// }

