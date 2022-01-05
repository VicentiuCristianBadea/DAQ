#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Wire.h>

#include "load_cell.h"
#include "sd_card.h"
#include "data.h"

#include "config/definitions.h"


using namespace std;

bool READ_FLAG = false;
char buffer[1000][100];


LoadCell loadCell_1;
LoadCell loadCell_2;
SD_card sd;
Data data;

int input_freq;
int counter = 0;

void setupSensors();
void setupSDCard();
void setupData();
void createSensor(LoadCell&, int);
void ledON();
void ledOFF();
void heartBeat();
void interruptReadData();
void readToBuffer();
bool checkBufferSize();


void setup()
{
  
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);


  setupSensors();
  setupSDCard();  
  setupData();


  TIM_TypeDef* Instance = TIM4;
  HardwareTimer * MyTim = new HardwareTimer(Instance);
 
  MyTim->pause();
  MyTim->refresh();
  MyTim->setOverflow(12500, MICROSEC_FORMAT);
  MyTim->attachInterrupt(interruptReadData);
  MyTim->resume();

  input_freq = MyTim->getTimerClkFreq()/ MyTim->getPrescaleFactor();
}


void loop()
{
  if(READ_FLAG){
    readToBuffer();
  }

  if(checkBufferSize()){
    sd.writeSD(buffer, TESTFILE);
  }
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
  sd.setupSD();
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
  READ_FLAG = true;
}

void readToBuffer(){
  data.emptyData();
  data.concatData(String(millis()));
  data.concatData(loadCell_1.readLoadString());
  data.concatData(loadCell_2.readLoadString());
  strcpy(buffer[counter], data.getData().c_str());
  counter++;
  READ_FLAG = false;
}

bool checkBufferSize(){
  if(counter > 995){
    counter = 0;
    return true;
  }
  return false;
}

