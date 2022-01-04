#ifndef SD_CARD_CPP
#define SD_CARD_CPP
#include <Arduino.h>
#include "SPI.h"
#include "SD.h"
#include "string.h"
#include "../config/definitions.cpp"

class SD_card
{

public:
    void setupSD()
    {
        SPI.setModule(2);

        Serial.print("Initializing SD card...");
        if (!SD.begin(SD_BEGIN_PIN))
        {
            while (1){
                Serial.println("initializing failed!");
                delay(INIT_DELAY);
            }       
        }
        Serial.println("Initialization done.");
    }

    void writeSD(char file[], String line)
    {
        myFile = SD.open(file, FILE_WRITE);
        String printFile = file;
        if (myFile)
        {
            Serial.print("Writing to " + printFile + ": " + line);
            myFile.println(line);
            myFile.close();
            Serial.println("done.");
        }
        else
        {
            Serial.println("error opening " + printFile);
        }
    }

    // void unitTestWrite(char file[], String line){
    //     myFile = file.open()
    // }

    File myFile;
};

#endif