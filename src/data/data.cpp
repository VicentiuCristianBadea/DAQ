#ifndef DATA_CPP
#define DATA_CPP
#include <Arduino.h>
#include "string.h"

class Data{

    
    public:
        Data(){
            data = "";
        }

        String concatData(String in){
            data = data + in + ", ";
            Serial.println("THIS IS IN CONCAT DATA: " + data);
            return data;
        };

        String getData(){
            Serial.println("THIS IS IN GETDATA: " + data);
            return data;
        };

        void emptyData(){
            data = "";
        }

    private:
        String data;

};





#endif