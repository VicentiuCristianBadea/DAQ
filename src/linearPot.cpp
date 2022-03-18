#include <Arduino.h>
#include <linearPot.h>

#include "config/definitions.h"
#include "config/globals.h"
#include "motor.h"

void LinearPot::setupLinearPot(const int pin_){
    pin = pin_;
    pinMode(pin_, INPUT);
}

void LinearPot::setData(int x){
    data = x;
}

int LinearPot::getData(){
    return data;
}

int LinearPot::getPin(){
    return pin;
}

void LinearPot::updateLinearPotData(LinearPot* l1, LinearPot* l2){
    if(READ_FLAG){
        l1->setData(analogRead(l1->getPin()));
        l2->setData(analogRead(l2->getPin()));
    }
}

boolean LinearPot::checkLinearPotDelta(LinearPot* l1, LinearPot* l2, MyMotor* m1, MyMotor* m2){
    if(abs(l1->getData() - l2->getData()) > MAX_LINEAR_POT_DELTA){
        m1->pauseMotor();
        m2->pauseMotor();
        return false;
    }
    return true;
}


