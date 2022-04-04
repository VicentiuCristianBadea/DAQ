
#include <driverInput.h>

#include "config/definitions.h"
#include "config/globals.h"


void DriverInput::setup(int pin_){
    pin = pin_;
    pinMode(pin_, INPUT);
}

void DriverInput::update(){
    if(READ_FLAG){
        int driverInput = 0;
        for(int i = 0; i<5; i++){
            driverInput += analogRead(getPin());
        }
        driverInput = driverInput/5;
        int mapped = map(driverInput, 0, 1023, 0, 90);
        if(mapped > 90){
            mapped = 90;
        }
        target = mapped;
    }
}

int DriverInput::getPin(){
    return pin;
}