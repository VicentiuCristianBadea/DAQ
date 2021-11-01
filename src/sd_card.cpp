#ifndef SD_CARD_CPP
#define SD_CARD_CPP
#include <Arduino.h>
#include "SD.h"
#include "SPI.h"
#include "string.h"

class SD_card
{

public:
    SD_card()
    {
        setupSD();
    }

    void setupSD()
    {
        Serial.print("Initializing SD card...");
        if (!SD.begin(10))
        {
            Serial.println("initializing failed!");
            while (1)
                ;
        }
        Serial.println("Initialization done.");
    }

    void writeSD(char file[])
    {
        myFile = SD.open(file, FILE_WRITE);
        String printFile = file;
        if (myFile)
        {
            Serial.print("Writing to " + printFile + "... ");
            for (int i = 0; i < 20; i++)
            {
                myFile.println(i);
            }
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