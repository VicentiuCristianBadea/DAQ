#include <Arduino.h>
#include <SPI.h>

#include "sd_card.h"

#include "config/definitions.h"

void SD_card::setupSD()
{
    // SPI.setModule(2);

    Serial.print("Initializing SD card...");
    if (!SD.begin(SPI_HALF_SPEED, SD_BEGIN_PIN))
    {
        while (1){
            Serial.println("initializing failed!");
            delay(INIT_DELAY);
        }
    }
    Serial.println("Initialization done.");
}

void SD_card::writeSD(const char *file, String line)
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

// void SD_card::unitTestWrite(char file[], String line){
//     myFile = file.open()
// }
