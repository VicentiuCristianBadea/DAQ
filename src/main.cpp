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

// PROTOTYPES
// void setupSensors();
// void setupSDCard();
void setupData();
void createSensor(LoadCell&, int);
void interruptReadData();
// void readToBuffer();
// bool checkBufferSize();
void m1_setMotor(int, int);
void m2_setMotor(int, int);
void readEncoder();
void readEncoder2();
void m1_computePID();
void m2_computePID();
// void moveToPosDEBUG();
// void moveSinWave();
void setupLoadCellTimer();
void setupMotor1();
void setupMotor2();

void setupDriverInput();
void updateDriverInput();
void printData();

boolean checkMotorAngleDelta();

// MOTOR 1
HardwareTimer *m1_timerLeft;
HardwareTimer *m1_timerRight;
uint32_t m1_channelLeft;
uint32_t m1_channelRight;
int m1_pos = 0;
long m1_prevT = 0;
float m1_eprev = 0;
float m1_eintegral = 0;
int target = 0;

// MOTOR 2
HardwareTimer *m2_timerLeft;
HardwareTimer *m2_timerRight;
uint32_t m2_channelLeft;
uint32_t m2_channelRight;
int m2_pos = 0;
long m2_prevT = 0;
float m2_eprev = 0;
float m2_eintegral = 0;

// LOAD CELLS
LoadCell loadCell_1;
LoadCell loadCell_2;

// SD CARD
SD_card sd;
Data data;
int counter = 0;
bool READ_FLAG = false;
char buffer[1000][100];

// Pot data
float EMA_a = 0.1;
int EMA_S = 0;

void setup()
{
  
  /*  Check if alive  */
  pinMode(PC13, OUTPUT);

  /*  Fix baudrate  */
  Serial.begin(115200);


  /*  Setup hardware */
  // setupSensors();
  // setupSDCard();  
  // setupData();

  /*  Setup timers */
  setupLoadCellTimer();

  /*  Setup motors */
  setupMotor1();
  setupMotor2();

  /*  Setup driver input resistance */
  setupDriverInput();
}


void loop()
{
  /*  LOADCELL CALIBRATION */
  // loadCell_1.calibrate();

  /*  LOADCELL READ DATA */
  // readToBuffer();

  /*  MOVE MOTOR TO POSITION - DEBUG */
  // moveToPosDEBUG();

  updateDriverInput();
  // if(checkMotorAngleDelta)
  m1_computePID();
  m2_computePID();

  printData();

  /*  MOVE MOTOR SIN WAVE - DEBUG */
  // moveSinWave();
}

void m1_computePID(){
  long currT = micros();
  float deltaT = ((float)(currT-m1_prevT))/1.0e6;
  m1_prevT = currT;
  int e = m1_pos-target;
  float dedt = (e - m1_eprev)/(deltaT);
  m1_eintegral = m1_eintegral + e*deltaT;
  float u = kp*e + kd*dedt + ki*m1_eintegral;
  float pwr = fabs(u);
  if(pwr>255){
    pwr = 255;
  }
  int dir = 1;
  if(u<0){
    dir = -1;
  }
  m1_setMotor(dir, pwr);
  m1_eprev = e;
}

void m2_computePID(){
  long currT = micros();
  float deltaT = ((float)(currT-m2_prevT))/1.0e6;
  m2_prevT = currT;
  int e = m2_pos-target;
  float dedt = (e - m2_eprev)/(deltaT);
  m2_eintegral = m2_eintegral + e*deltaT;
  float u = kp*e + kd*dedt + ki*m2_eintegral;
  float pwr = fabs(u);
  if(pwr>255){
    pwr = 255;
  }
  int dir = 1;
  if(u<0){
    dir = -1;
  }
  m2_setMotor(dir, pwr);
  m2_eprev = e;
}

void printData(){
  Serial.print(target);
  Serial.print(" ");
  Serial.print(m1_pos);
  Serial.print(" ");
  Serial.print(m2_pos);
  Serial.println();
}

void readEncoder(){
  int b = digitalRead(m1_encoder_B);
  if(b>0){
    m1_pos++;
  }
  else{
    m1_pos--;
  }
}

void readEncoder2(){
  int b = digitalRead(m2_encoder_B);
  if(b>0){
    m2_pos++;
  }else{
    m2_pos--;
  }
}

boolean checkMotorAngleDelta(){
  if(abs(m1_pos - m2_pos) > 3){
    return false;
  }
  return true;
}

void m1_setMotor(int dir, int pwr){
  analogWrite(m1_motorPin, pwr);
  if(dir == turnClockWise){   // Turn right
    m1_timerLeft->pauseChannel(m1_channelLeft);
    m1_timerRight->resume();
  }else if(dir == turnCounterClockWise){ //Turn left
    m1_timerRight->pauseChannel(m1_channelRight);
    m1_timerLeft->resume();
  }else{
    m1_timerLeft->pauseChannel(m1_channelLeft);
    m1_timerRight->pauseChannel(m1_channelRight);
  }
}

void m2_setMotor(int dir, int pwr){
  analogWrite(m2_motorPin, pwr);
  if(dir == turnClockWise){   // Turn right
    m2_timerLeft->pauseChannel(m2_channelLeft);
    m2_timerRight->resume();
  }else if(dir == turnCounterClockWise){ //Turn left
    m2_timerRight->pauseChannel(m2_channelRight);
    m2_timerLeft->resume();
  }else{
    m2_timerLeft->pauseChannel(m2_channelLeft);
    m2_timerRight->pauseChannel(m2_channelRight);
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

void interruptReadData(){
  READ_FLAG = true;
}

// void readToBuffer(){
//   if(READ_FLAG){
//     data.emptyData();
//     data.concatData(String(millis()));
//     data.concatData(loadCell_1.readLoadString());
//     data.concatData(loadCell_2.readLoadString());
//     if(counter < 1000){
//       strcpy(buffer[counter], data.getData().c_str());
//       counter++;
//     }
//     Serial.println(data.getData());
//     READ_FLAG = false;
//   }

//   if(checkBufferSize()){
//     sd.writeSD(buffer, TESTFILE);
//   }
// }

void setupDriverInput(){
  pinMode(PB1, INPUT);

  EMA_S = analogRead(PB1);
}

void updateDriverInput(){
  if(READ_FLAG){
    int driverInput = 0;
    for(int i = 0; i < 5; i++){
      driverInput += analogRead(PB1);
    //   // EMA_S += (EMA_a*driverInput) + ((1-EMA_a)*EMA_S);
    }
    driverInput = driverInput/5;
   
    int mapped = map(driverInput, 0, 200, 0, 90);
    // int mapped = map(EMA_S, 0, 200, 0 ,90);
    if(mapped > 90){
      mapped = 90;
    }
    target = mapped;
    READ_FLAG = false;
  }
}

// bool checkBufferSize(){
//   if(counter > 995){
//     counter = 0;
//     return true;
//   }
//   return false;
// }


// void moveToPosDEBUG(){
//   Serial.println("Please enter a target position from 0 to 180 degrees: ");
//   char buffer[] = {' ', ' ', ' '};
//   int incomingValue;

//   while(!Serial.available());
//   Serial.readBytesUntil('\n', buffer, 3);
//   incomingValue = atoi(buffer);
//   Serial.println(incomingValue);

//   target = incomingValue;

//   Serial.print("Target is: ");
//   Serial.print(target);
//   Serial.println();

//   Serial.println("Enter the character 'g' to move the motor.");

//   while(Serial.available() == 0){
//   }
//   int myData = Serial.read();
 
//   if(myData == 'g'){
//     while(pos < target){
//       computePID();
//     }
//   }
// }

// void moveSinWave(){
//   target = 250*sin(prevT/1.0e6);
//   computePID();
//   delay(0.5);
// }


void setupLoadCellTimer(){
  TIM_TypeDef* Instance = LOADCELL_TIMER;
  HardwareTimer * MyTim = new HardwareTimer(Instance);
 
  MyTim->pause();
  MyTim->refresh();
  MyTim->setOverflow(LOADCELL_TIMER_FREQ, HERTZ_FORMAT);
  MyTim->attachInterrupt(interruptReadData);
  MyTim->resume();
}

void setupMotor1(){
  TIM_TypeDef *InstanceLeft = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(m1_left), PinMap_PWM);
  m1_channelLeft = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m1_left), PinMap_PWM));
  m1_timerLeft = new HardwareTimer(InstanceLeft);

  TIM_TypeDef *InstanceRight = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(m1_right), PinMap_PWM);
  m1_channelRight = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m1_right), PinMap_PWM));
  m1_timerRight = new HardwareTimer(InstanceRight);

  m1_timerRight->setPWM(m1_channelRight, m1_right, 1000, 100);
  m1_timerRight->pauseChannel(m1_channelRight);
  m1_timerLeft->setPWM(m1_channelLeft, m1_left, 1000, 100);

  pinMode(m1_motorPin, OUTPUT);
  analogWrite(m1_motorPin, 0);

  pinMode(m1_encoder_A, INPUT);
  pinMode(m1_encoder_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(m1_encoder_A), readEncoder, RISING);
}

void setupMotor2(){
  TIM_TypeDef *InstanceLeft = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(m2_left), PinMap_PWM);
  m2_channelLeft = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m2_left), PinMap_PWM));
  m2_timerLeft = new HardwareTimer(InstanceLeft);

  TIM_TypeDef *InstanceRight = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(m2_right), PinMap_PWM);
  m2_channelRight = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(m2_right), PinMap_PWM));
  m2_timerRight = new HardwareTimer(InstanceRight);

  m2_timerRight->setPWM(m2_channelRight, m2_right, 1000, 100);
  m2_timerRight->pauseChannel(m2_channelRight);
  m2_timerLeft->setPWM(m2_channelLeft, m2_left, 1000, 100);

  pinMode(m2_motorPin, OUTPUT);
  analogWrite(m2_motorPin, 0);

  pinMode(m2_encoder_A, INPUT);
  pinMode(m2_encoder_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(m2_encoder_A), readEncoder2, RISING);
}

// TODO:
// Change readEncoder to readEncoder1 
// Create readEncoder2
// Add attachInterrupt(digitalPinToInterrupt(m2_encoder_A), readEncoder2, RISING);
// Subtract encoder values of m1 and m2, and if too different - out of synch, throw exception


