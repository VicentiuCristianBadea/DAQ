#include <Arduino.h>
#include <string.h>

#include "data.h"

Data::Data()
{
    data = "";
}

void Data::concatData(String in){
    data = data + in + ", ";
};

String Data::getData(){
    return data;
};

void Data::emptyData(){
    data = "";
}





