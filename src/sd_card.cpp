#ifndef SD_CARD_CPP
#define SD_CARD_CPP
#include <Arduino.h>
#include "SD.h"
#include "SPI.h"
#include "string.h"

class SD_card
{

public:
    void setupSD()
    {
        Serial.print("Initializing SD card...");
        if (!SD.begin(53))
        {
            while (1){
                Serial.println("initializing failed!");
                delay(1000);
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

    File myFile;
};

#endif