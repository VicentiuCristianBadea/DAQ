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
#include "hallEffect.h"
#include "sensor.h"

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

void setupHallEffect1();
void setupHallEffect2();
void updateHallEffectData();

boolean checkLinearPotDelta();

void printDataSerial();
void printDataPlotter();

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
HallEffect hallEffect1; 
HallEffect hallEffect2;

// INIT GLOBALS
int target = 0;
int counter = 0;
bool READ_FLAG = false;
char buffer[1000][100];

// MOTOR POT DATA
float EMA_a = 0.1;
int EMA_S = 0;

void setup()
{
  /*  Application  */
  pinMode(LED_PIN, OUTPUT);

  /*  Fix baudrate  */
  Serial.begin(115200);

  /*  Setup timers  */
  setupReadDataTimer();

  /*  Setup modules */
  // setupSDCard();
  setupMotor1();
  setupMotor2();
  setupLinearPot1();
  setupLinearPot2();
  setupHallEffect1();
  setupHallEffect2();
  setupDriverInput();

  m1.setMotorToZero(&hallEffect1);
  m2.setMotorToZero(&hallEffect2);
}

void loop()
{
  driverInput.update();
  hallEffect1.update();
  hallEffect2.update();
  LinearPot::update(&linearPot1, &linearPot2);
  READ_FLAG = false;

  if(LinearPot::checkLinearPotDelta(&linearPot1, &linearPot2, &m1, &m2)){
    m1.computePID();
    m2.computePID();
  }

  printDataPlotter();
}

void printDataSerial(){
  // Serial.print(millis());
  Serial.print(" TV ");
  Serial.print(target);
  Serial.print(", ME1 ");
  Serial.print(m1.getPos());
  Serial.print(", ME2 ");
  Serial.print(m2.getPos());
  Serial.print(", LP1 ");
  Serial.print(linearPot1.getData());
  Serial.print(", LP2 ");
  Serial.print(linearPot2.getData());
  Serial.print(", HE1 ");
  Serial.print(hallEffect1.getData());
  Serial.print(", HE2 ");
  Serial.print(hallEffect2.getData());
  Serial.println();
}

void printDataPlotter(){
  Serial.print(target);
  Serial.print(",");
  Serial.print(m1.getPos());
  Serial.print(",");
  Serial.print(m2.getPos());
  Serial.print(",");
  Serial.print(linearPot1.getData());
  Serial.print(",");
  Serial.print(linearPot2.getData());
  Serial.print(",");
  Serial.print(hallEffect1.getData());
  Serial.print(",");
  Serial.print(hallEffect2.getData());
  Serial.println();
}

// ---------------------------------
// SETUP

void setupSDCard(){
  sd = SD_card();
  sd.setupSD();
}

// Driver adjustable rotary potentiometer
void setupDriverInput(){
  driverInput = DriverInput();
  driverInput.setup(di_pin);
}

// Linear Potentiometer on Shock 1
void setupLinearPot1(){
  linearPot1 = LinearPot();
  linearPot1.setup(l1_pin);
}

// Linear Potentiometer on Shock 2
void setupLinearPot2(){
  linearPot2 = LinearPot();
  linearPot2.setup(l2_pin);
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

void setupHallEffect1(){
  hallEffect1 = HallEffect();
  hallEffect1.setup(HALL_EFFECT_1);
}

void setupHallEffect2(){
  hallEffect2 = HallEffect();
  hallEffect2.setup(HALL_EFFECT_2);
}

// Create and setup Motor 1
void setupMotor1(){
  m1 = MyMotor();
  m1.setup(m1_motorPin, m1_left, m1_right, m1_encoder_A, m1_encoder_B);
  attachInterrupt(digitalPinToInterrupt(m1.getEncoderA()), m1ReadEncoder, RISING);
}

// Create and setup Motor 2
void setupMotor2(){
  m2 = MyMotor();
  m2.setup(m2_motorPin, m2_left, m2_right, m2_encoder_A, m2_encoder_B);
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