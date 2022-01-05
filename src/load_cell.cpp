#include <Arduino.h>
#include <HX711.h>
#include <string.h>

#include "load_cell.h"

using namespace std;

void LoadCell::setupLoadCell(int dout_pin, int sck_pin, int timeout){
    _dout_pin = dout_pin;
    _sck_pin = sck_pin;
    _timeout = timeout;
}

void LoadCell::loadCellBegin(){
    _scale.begin(_dout_pin, _sck_pin);
    Serial.println("Beginning LoadCell");
}

double LoadCell::readLoadDouble(){
    long int reading;
    if(_scale.is_ready()){
        reading = _scale.read();
    }
    return reading;
}

String LoadCell::readLoadString(){
    double temp = readLoadDouble();
    String temp_string;
    temp_string = String(temp, 8);
    return temp_string;
}
