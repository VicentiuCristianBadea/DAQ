#include <Arduino.h>
#include <Wire.h>

#include "motor.h"

#include "config/definitions.h"
#include "config/globals.h"
#include "hallEffect.h"

using namespace std;

void MyMotor::setup(const int mp, const int lp, const int rp, const int eA, const int eB){
  motorPin = mp;
  leftPin = lp;
  rightPin = rp;
  encoderA = eA;
  encoderB = eB;
  pos, prevT, eprev, eintegral = 0;

  setupMotorTimers();
  setupMotorPins();
}

void MyMotor::setupMotorTimers(){
  TIM_TypeDef *InstanceLeft = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(leftPin), PinMap_PWM);
  channelLeft = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(leftPin), PinMap_PWM));
  timerLeft = new HardwareTimer(InstanceLeft);

  TIM_TypeDef *InstanceRight = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(rightPin), PinMap_PWM);
  channelRight = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(rightPin), PinMap_PWM));
  timerRight = new HardwareTimer(InstanceRight);

  timerRight->setPWM(channelRight, rightPin, 1000, 100);
  timerRight->pauseChannel(channelRight);
  timerLeft->setPWM(channelLeft, leftPin, 1000, 100);
}

void MyMotor::setupMotorPins(){
  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0);
  pinMode(encoderA, INPUT);
  pinMode(encoderB, INPUT);
}

void MyMotor::setMotor(int dir, int pwr){
  analogWrite(motorPin, pwr);
  if(dir == turnClockWise){   // Turn right
      timerLeft->pauseChannel(channelLeft);
      timerRight->resume();
  }else if(dir == turnCounterClockWise){ //Turn left
      timerRight->pauseChannel(channelRight);
      timerLeft->resume();
  }else{
      timerLeft->pauseChannel(channelLeft);
      timerRight->pauseChannel(channelRight);
  }
}

void MyMotor::setMotorToZero(HallEffect* hallEffect){
  analogWrite(motorPin, 100);
  timerLeft->pauseChannel(channelLeft);
  timerRight->resume();
  hallEffect->setData(analogRead(hallEffect->getPin()));
  while(hallEffect->getData() > 15){
    // Keep turning until magnet and hall effect air gap minimized
    hallEffect->update();
  }

  pauseMotor();
  resetEncoder();
}

void MyMotor::resetEncoder(){
  pos = 0;
}

void MyMotor::computePID(){
  long currT = micros();
  float deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;
  int e = getError();
  float dedt = (e - eprev)/(deltaT); //  Error rate of change

  eintegral = eintegral + e*deltaT;
  float u = kp*e + kd*dedt + ki*eintegral; //  Control signal

  float pwr = fabs(u);
  pwr = PIDlimitPower(pwr);
  
  int dir = PIDgetDirection(u);

  setMotor(dir, pwr);
  eprev = e;
}

void MyMotor::addPos(){
  pos++;
}

void MyMotor::subPos(){
  pos--;
}

void MyMotor::pauseMotor(){
  timerLeft->pauseChannel(channelLeft);
  timerRight->pauseChannel(channelRight);
}

float MyMotor::PIDlimitPower(float power){
  if(power>PID_MAXPOWER){
    return float(PID_MAXPOWER);
  }
  return power;
}

int MyMotor::PIDgetDirection(float u){
  int dir = 1;
  if(u<0){
    return -1;
  }
  else{
    return 1;
  }
}

int MyMotor::getError(){
  return pos-target;
}


boolean MyMotor::checkMotorAngleDelta(MyMotor m1, MyMotor m2){
  if(abs(m1.getPos() - m2.getPos()) > 5){
    m1.pauseMotor();
    m2.pauseMotor();
    return false;
  }
  return true;
}

int MyMotor::getPos(){
  return pos;
}

int MyMotor::getEncoderA(){
  return encoderA;
}

int MyMotor::getEncoderB(){
  return encoderB;
}
