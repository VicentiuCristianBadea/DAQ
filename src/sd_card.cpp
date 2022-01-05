#include <Arduino.h>
#include <SPI.h>

#include "sd_card.h"

#include "config/definitions.h"

void SD_card::setupSD(const char *file)
{
    saving = false;
    dataCount = 0;
    String printFile = file;

    Serial.print("Initializing SD card...");
    if (!SD.begin(SPI_HALF_SPEED, SD_BEGIN_PIN))
    {
        while (1){
            Serial.println("initializing failed!");
            delay(INIT_DELAY);
        }
    }
    myFile = SD.open(file, FILE_WRITE);
    Serial.println("Initialization done.");
}

void SD_card::writeSD(String line)
{
    if (myFile)
    {
        Serial.print("Writing to " + printFile + ": " + line);
        myFile.println(line);
        // myFile.close();
        Serial.println("done.");
    }
    else
    {
        Serial.println("error opening " + printFile);
    }
}

// void SD_card::incrementDataCount(){
//     if(dataCount < 1000){
//         dataCount++;
//     }else{
//         dataCount = 0;
//         saving = true;
//         save();
//     }
// }

// void SD_card::save(){
//     myFile.close();
//     myFile = SD.open()
// }

// void SD_card::unitTestWrite(char file[], String line){
//     myFile = file.open()
// }
