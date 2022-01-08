#include <Arduino.h>
#include <HX711.h>
#include <string.h>

#include "load_cell.h"
#include "config/definitions.h"

using namespace std;

void LoadCell::setupLoadCell(int dout_pin, int sck_pin, int timeout){
    _dout_pin = dout_pin;
    _sck_pin = sck_pin;
    _timeout = timeout;
    _calibration_factor = -7020;
}

void LoadCell::loadCellBegin(){
    _scale.begin(_dout_pin, _sck_pin);
    _scale.set_scale(_calibration_factor);
    _scale.tare();
    _scale.read_average();

    Serial.println("Beginning LoadCell");
}

double LoadCell::readLoadDouble(){
    double reading;
    if(_scale.is_ready()){
        reading = _scale.get_units();
    }
    return reading;
}

String LoadCell::readLoadString(){
    double temp = readLoadDouble();
    return String(temp);
}

void LoadCell::calibrate(){
    _scale.set_scale(_calibration_factor);
    Serial.print("Reading: ");
    Serial.print(_scale.get_units(), 10);
    Serial.print(" kg");
    Serial.print(" CALIBRATION FACTOR: ");
    Serial.print(_calibration_factor);
    Serial.println();

    if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      _calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      _calibration_factor -= 10;
  }
}
