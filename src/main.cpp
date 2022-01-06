#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Wire.h>
#include <Servo.h>

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
int pos = 0;

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

Servo myservo;

bool changeOrientation = true;
#define left PB3
#define right PB4
HardwareTimer *MotorLeft;
HardwareTimer *MotorRight;

uint32_t channelLeft;
uint32_t channelRight;


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
  MyTim->setOverflow(80, HERTZ_FORMAT);
  MyTim->attachInterrupt(interruptReadData);
  MyTim->resume();

  input_freq = MyTim->getTimerClkFreq()/ MyTim->getPrescaleFactor();

  TIM_TypeDef *InstanceLeft = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(left), PinMap_PWM);
  channelLeft = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(left), PinMap_PWM));
  MotorLeft = new HardwareTimer(InstanceLeft);

  TIM_TypeDef *InstanceRight = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(right), PinMap_PWM);
  channelRight = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(right), PinMap_PWM));
  MotorRight = new HardwareTimer(InstanceRight);
  

  MotorRight->setPWM(channelRight, right, 1000, 100);
  MotorRight->pauseChannel(channelRight);
  MotorLeft->setPWM(channelLeft, left, 1000, 100);
  

  pinMode(PA15, OUTPUT);
  analogWrite(PA15, 0);
}


void loop()
{
  // if(READ_FLAG){
  //   readToBuffer();
  // }

  // if(checkBufferSize()){
  //   sd.writeSD(buffer, TESTFILE);
  // }

  analogWrite(PA15, 255);
  delay(1000);
  analogWrite(PA15, 0);
  delay(1000);

  if(changeOrientation){
    MotorLeft->pauseChannel(channelLeft);
    delay(100);
    MotorRight->resume();
    changeOrientation = false;
  }else{
    MotorRight->pauseChannel(channelRight);
    delay(100);
    MotorLeft->resume();
    changeOrientation = true;
  }
  delay(10);

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

