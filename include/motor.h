#ifndef MOTOR_H
#define MOTOR_H

#include <string.h>
#include <Wire.h>

#include "data.h"
#include "hallEffect.h"

class MyMotor{

    public:
        void setup(int, int, int, int, int);
        void setMotor(int, int);
        void computePID();
        void addPos();
        void subPos();

        void addPos2();
        void subPos2();
        
        int getPos();
        int getEncoderA();
        int getEncoderB();
        void pauseMotor();
        void setMotorToZero(HallEffect*);

        static boolean checkMotorAngleDelta(MyMotor m1, MyMotor m2);

    private:

        void setupMotorTimers();
        void setupMotorPins();
        void resetEncoder();

        
        int getError();
        int PIDgetDirection(float);
        float PIDlimitPower(float);
        
        HardwareTimer *timerLeft;
        HardwareTimer *timerRight;
        uint32_t channelLeft;
        uint32_t channelRight;

        float pos;
        long prevT;
        float eprev;
        float eintegral;
        int motorPin, leftPin, rightPin, encoderA, encoderB;
};



#endif