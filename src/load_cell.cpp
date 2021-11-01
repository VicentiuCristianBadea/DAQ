#ifndef LOAD_CELL_CPP
#define LOAD_CELL_CPP
#include <Arduino.h>


class LoadCell{
    
    public: 
        
        LoadCell(int p, int t){
            pin   = p;
            type  = t;
            setPinMode();
        }

        void setPinMode(){
            pinMode(pin, type);
        }

        void setLow(){
            digitalWrite(pin, LOW);
        }
        void setHigh(){
            digitalWrite(pin, HIGH);
        }

        void blinkLED(){
            setLow();
            delay(1000);
            setHigh();
            delay(1000);
        }

        int pin;
        int type;
};

#endif
