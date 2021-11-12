#ifndef DATA_CPP
#define DATA_CPP
#include <Arduino.h>
#include "string.h"

class Data{

    public:
        static void concatData(String in){
            data = data + in + ", ";
        };

        static String getData(){
            return data;
        };

        static void emptyData(){
            data = "";
        }

    private:
        static String data;
};





#endif