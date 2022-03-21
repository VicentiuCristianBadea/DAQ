#include <hallEffect.h>

#include "Arduino.h"
#include "config/definitions.h"
#include "config/globals.h"

void HallEffect::setup(const int pin_){
    pin = pin_;
    pinMode(pin, INPUT);
}

void HallEffect::update(){
    if(READ_FLAG){
        setData(analogRead(getPin()));
    }
}

void HallEffect::setData(int x){
    data = x;
}

int HallEffect::getData(){
    return data;
}

int HallEffect::getPin(){
    return pin;
}