#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Wire.h>
#include <Servo.h>

#include "loadCell.h"
#include "sdCard.h"
#include "data.h"
#include "motor.h"
#include "linearPot.h"
#include "driverInput.h"

#include "config/definitions.h"
#include "config/globals.h"

using namespace std;

// SETUP PROTOTYPES
void setupSDCard();
void setupReadDataTimer();
void setupMotor1();
void setupMotor2();
void setupLinearPot1();
void setupLinearPot2();
void setupDriverInput();

// HELPER PROTOTYPES
void printData();
void readToBuffer();
bool checkBufferSize();

// CALLBACK PROTOTYPES
void m1ReadEncoder();
void m2ReadEncoder();
void interruptReadData();

// OBJECT DECLARATIONS
MyMotor m1;
MyMotor m2;
SD_card sd;
Data data;
LinearPot linearPot1;
LinearPot linearPot2;
DriverInput driverInput;

// INIT GLOBALS
int target = 0;
int counter = 0;
bool READ_FLAG = false;
char buffer[1000][100];

void setup()
{
  /*  Application  */
  pinMode(PC13, OUTPUT);

  /*  Fix baudrate  */
  Serial.begin(115200);

  /*  Setup timers  */
  setupReadDataTimer();

  /*  Setup modules */
  setupSDCard();
  setupMotor1();
  setupMotor2();
  setupLinearPot1();
  setupLinearPot2();
  setupDriverInput();
}

void loop()
{
  DriverInput::updateDriverInput(driverInput);
  LinearPot::updateLinearPotData(linearPot1, linearPot2);
  
  if(LinearPot::checkLinearPotDelta(linearPot1, linearPot2) && MyMotor::checkMotorAngleDelta(m1, m2)){
    m1.computePID();
    m2.computePID();
  }

  printData();
}

void printData(){
  Serial.print(millis());
  Serial.print(" Target value: ");
  Serial.print(target);
  Serial.print(" | M1 Encoder: ");
  Serial.print(m1.getPos());
  Serial.print(" | M2 Encoder: ");
  Serial.print(m2.getPos());
  Serial.print(" | LinearPot1: ");
  Serial.print(linearPot1.getData());
  Serial.print(" | LinearPot2: ");
  Serial.print(linearPot2.getData());
  Serial.println();
}

// ---------------------------------
// SETUP

// 32GB SD Module 
void setupSDCard(){
  sd = SD_card();
  sd.setupSD();
}

// Driver adjustable rotary potentiometer
void setupDriverInput(){
  driverInput = DriverInput();
  driverInput.setupDriverInput(di_pin);
}

// Linear Potentiometer on Shock 1
void setupLinearPot1(){
  linearPot1 = LinearPot();
  linearPot1.setupLinearPot(l1_pin);
}

// Linear Potentiometer on Shock 2
void setupLinearPot2(){
  linearPot2 = LinearPot();
  linearPot2.setupLinearPot(l2_pin);
}

//  Create and set timer to set READ_FLAG to true 
void setupReadDataTimer(){
  TIM_TypeDef* Instance = READ_DATA_TIMER;
  HardwareTimer * MyTim = new HardwareTimer(Instance);
 
  MyTim->pause();
  MyTim->refresh();
  MyTim->setOverflow(READ_DATA_TIMER_FREQ, HERTZ_FORMAT);
  MyTim->attachInterrupt(interruptReadData);
  MyTim->resume();
}

// Create and setup Motor 1
void setupMotor1(){
  m1 = MyMotor();
  m1.setupMotor(m1_motorPin, m1_left, m1_right, m1_encoder_A, m1_encoder_B);
  attachInterrupt(digitalPinToInterrupt(m1.getEncoderA()), m1ReadEncoder, RISING);
}

// Create and setup Motor 2
void setupMotor2(){
  m2 = MyMotor();
  m2.setupMotor(m2_motorPin, m2_left, m2_right, m2_encoder_A, m2_encoder_B);
  attachInterrupt(digitalPinToInterrupt(m2.getEncoderA()), m2ReadEncoder, RISING);
}

// ---------------------------------
// CALLBACKS

// Set READ_FLAG to true and allow sensor reading
void interruptReadData(){
  READ_FLAG = true;
}

// Read data from encoder 1 and increment or decrement its position
void m1ReadEncoder(){
  int b = digitalRead(m1.getEncoderB());
  if(b>0){
      m1.addPos();
  }
  else{
      m1.subPos();
  }
}

// Read data from encoder 2 and increment or decrement its position
void m2ReadEncoder(){
  int b = digitalRead(m2.getEncoderB());
  if(b>0){
      m2.addPos();
  }
  else{
      m2.subPos();
  }
}