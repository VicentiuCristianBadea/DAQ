#include <Arduino.h>
#include <HX711.h>
#include <string.h>

#include "load_cell.h"


using namespace std;

void LoadCell::setupLoadCell(int dout_pin, int sck_pin, int timeout, int timer){
    _dout_pin = dout_pin;
    _sck_pin = sck_pin;
    _timeout = timeout;
    _timer = timer;
}

void LoadCell::loadCellBegin(){
    _scale.begin(_dout_pin, _sck_pin);
    Serial.println("Beginning LoadCell");
}

double LoadCell::readLoadDouble(){
    long int reading;
    if(_scale.wait_ready_timeout(_timeout)){
        reading = _scale.read();
        Serial.println("HX711 reading: ");
        Serial.println(reading);
    }else{
        Serial.println("HX711 not found.");
    }
    delay(_timer);
    return reading;
}

String LoadCell::readLoadString(Data data){
    double temp = readLoadDouble();
    String temp_string;
    temp_string = String(temp, 8);
    Serial.println("THIS IS STRING VALUE: " + temp_string);
    return data.concatData(temp_string);
}
