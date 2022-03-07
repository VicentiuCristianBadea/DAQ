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
void setupSensors();
void setupSDCard();
void setupData();
void createSensor(LoadCell&, int);
void interruptReadData();
void readToBuffer();
bool checkBufferSize();
void setMotor(int, int);
void readEncoder();
void computePID();
void moveToPosDEBUG();
void moveSinWave();
void setupLoadCellTimer();
void setupMotor1();

void setupDriverInput();
void updateDriverInput();

// MOTOR 1
HardwareTimer *MotorLeft;
HardwareTimer *MotorRight;
uint32_t channelLeft;
uint32_t channelRight;
int pos = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;
int target = 0;

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
  setupSensors();
  setupSDCard();  
  setupData();

  /*  Setup timers */
  setupLoadCellTimer();

  /*  Setup motors */
  setupMotor1();

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
  computePID();

  /*  MOVE MOTOR SIN WAVE - DEBUG */
  // moveSinWave();
}

void computePID(){
  long currT = micros();
  float deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;

  int e = pos-target;

  float dedt = (e - eprev)/(deltaT);

  eintegral = eintegral + e*deltaT;

  float u = kp*e + kd*dedt + ki*eintegral;

  float pwr = fabs(u);
  if(pwr>255){
    pwr = 255;
  }

  int dir = 1;
  if(u<0){
    dir = -1;
  }

  setMotor(dir, pwr);

  eprev = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}

void readEncoder(){
  int b = digitalRead(m1_encoder_B);
  if(b>0){
    pos++;
  }
  else{
    pos--;
  }
}

void setMotor(int dir, int pwr){
  analogWrite(motorPin, pwr);
  if(dir == turnClockWise){   // Turn right
    MotorLeft->pauseChannel(channelLeft);
    MotorRight->resume();
  }else if(dir == turnCounterClockWise){ //Turn left
    MotorRight->pauseChannel(channelRight);
    MotorLeft->resume();
  }else{
    MotorLeft->pauseChannel(channelLeft);
    MotorRight->pauseChannel(channelRight);
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

void readToBuffer(){
  if(READ_FLAG){
    data.emptyData();
    data.concatData(String(millis()));
    data.concatData(loadCell_1.readLoadString());
    data.concatData(loadCell_2.readLoadString());
    if(counter < 1000){
      strcpy(buffer[counter], data.getData().c_str());
      counter++;
    }
    Serial.println(data.getData());
    READ_FLAG = false;
  }

  if(checkBufferSize()){
    sd.writeSD(buffer, TESTFILE);
  }
}

void setupDriverInput(){
  pinMode(PB1, INPUT);
}

void updateDriverInput(){
  if(READ_FLAG){
    // analogReadResolution(16);
    int driverInput = 0;
    for(int i = 0; i < 10; i++){
      driverInput += analogRead(PB1);
      // EMA_S += (EMA_a*driverInput) + ((1-EMA_a)*EMA_S);
    }
    driverInput = driverInput/10;
   
    int mapped = map(driverInput, 0,200, 0, 90);
    if(mapped > 90){
      mapped = 90;
    }
    target = mapped;
    // Serial.println(mapped);
    READ_FLAG = false;
  }
}

bool checkBufferSize(){
  if(counter > 995){
    counter = 0;
    return true;
  }
  return false;
}


void moveToPosDEBUG(){
  Serial.println("Please enter a target position from 0 to 180 degrees: ");
  char buffer[] = {' ', ' ', ' '};
  int incomingValue;

  while(!Serial.available());
  Serial.readBytesUntil('\n', buffer, 3);
  incomingValue = atoi(buffer);
  Serial.println(incomingValue);

  target = incomingValue;

  Serial.print("Target is: ");
  Serial.print(target);
  Serial.println();

  Serial.println("Enter the character 'g' to move the motor.");

  while(Serial.available() == 0){
  }
  int myData = Serial.read();
 
  
  if(myData == 'g'){
    while(pos < target){
      computePID();
    }
  }
}

void moveSinWave(){
  target = 250*sin(prevT/1.0e6);
  computePID();
  delay(0.5);
}


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
  TIM_TypeDef *InstanceLeft = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(left), PinMap_PWM);
  channelLeft = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(left), PinMap_PWM));
  MotorLeft = new HardwareTimer(InstanceLeft);

  TIM_TypeDef *InstanceRight = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(right), PinMap_PWM);
  channelRight = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(right), PinMap_PWM));
  MotorRight = new HardwareTimer(InstanceRight);
  

  MotorRight->setPWM(channelRight, right, 1000, 100);
  MotorRight->pauseChannel(channelRight);
  MotorLeft->setPWM(channelLeft, left, 1000, 100);

  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0);

  pinMode(m1_encoder_A, INPUT);
  pinMode(m1_encoder_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(m1_encoder_A), readEncoder, RISING);
}



// TODO:
// Change readEncoder to readEncoder1 
// Create readEncoder2
// Add attachInterrupt(digitalPinToInterrupt(m2_encoder_A), readEncoder2, RISING);
// Subtract encoder values of m1 and m2, and if too different - out of synch, throw exception


