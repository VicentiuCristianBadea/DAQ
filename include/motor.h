#ifndef MOTOR_H
#define MOTOR_H

#include <string.h>
#include <Wire.h>

#include "data.h"

class MyMotor{

    public:
        void setupMotor(int, int, int, int, int);
        void setMotor(int, int);
        void computePID();
        void addPos();
        void subPos();
        
        int getPos();
        int getEncoderA();
        int getEncoderB();
        void pauseMotor();

        static boolean checkMotorAngleDelta(MyMotor m1, MyMotor m2);

    private:

        void setupMotorTimers();
        void setupMotorPins();
        
        int getError();
        int PIDgetDirection(float);
        float PIDlimitPower(float);
        
        HardwareTimer *timerLeft;
        HardwareTimer *timerRight;
        uint32_t channelLeft;
        uint32_t channelRight;

        int pos;
        long prevT;
        float eprev;
        float eintegral;
        int motorPin, leftPin, rightPin, encoderA, encoderB;
};



#endif