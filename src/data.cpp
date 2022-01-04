#include <Arduino.h>
#include <string.h>

#include "data.h"

Data::Data()
{
    data = "";
}

String Data::concatData(String in){
    data = data + in + ", ";
    Serial.println("THIS IS IN CONCAT DATA: " + data);
    return data;
};

String Data::getData(){
    Serial.println("THIS IS IN GETDATA: " + data);
    return data;
};

void Data::emptyData(){
    data = "";
}





