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

  

  // TIM_TypeDef *Instance2 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB4), PinMap_PWM);
  // uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB4), PinMap_PWM));

  // HardwareTimer *MotorTimer = new HardwareTimer(Instance2);
  
  // MotorTimer->setMode(channel, TIMER_OUTPUT_COMPARE_PWM1, PB4);
  // MotorTimer->setOverflow(100000, MICROSEC_FORMAT);
  // MotorTimer->setCaptureCompare(channel, 50, PERCENT_COMPARE_FORMAT);
  
  
  // MotorTimer->setPWM(channel, PB4, 5, 10);
  // MotorTimer->pause();
  // MotorTimer->refresh();

  // myservo.attach(PA15);

  pinMode(PA15, OUTPUT);
  analogWrite(PA15, LOW);
}


void loop()
{
  // if(READ_FLAG){
  //   readToBuffer();
  // }

  // if(checkBufferSize()){
  //   sd.writeSD(buffer, TESTFILE);
  // }

  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // myservo.write(250);
  // delay(1000);
  // myservo.write(0);
  // delay(1000);

  analogWrite(PA15, 255);
  delay(1000);
  analogWrite(PA15, 0);
  delay(1000);

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

