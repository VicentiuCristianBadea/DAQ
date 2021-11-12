#ifndef LOAD_CELL_CPP
#define LOAD_CELL_CPP
#include <Arduino.h>
#include "HX711.h"
#include "string.h"



using namespace std;

class LoadCell
{

public:
    
    void setupLoadCell(int dout_pin, int sck_pin, int timeout, int timer){
        _dout_pin = dout_pin;
        _sck_pin = sck_pin;
        _timeout = timeout;
        _timer = timer;
    }

    void loadCellBegin(){
        _scale.begin(_dout_pin, _sck_pin);
    }

    double readLoadDouble(){
        long int reading;
        if(_scale.wait_ready_timeout(_timeout)){
            reading = _scale.read();
            Serial.println("HX711 reading: ");
            Serial.println(reading);
        }else{
            Serial.println("HX711 not found.");
        }

        return reading;
    }

    String readLoadString(){
        
        double temp = readLoadDouble();
        String temp_string;
        temp_string = String(temp, 8);
        return temp_string;
    }


    private:

        int _dout_pin;
        int _sck_pin;
        int _timeout;
        int _timer;

        HX711 _scale;

};

#endif
