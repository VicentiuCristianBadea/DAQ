#include <Arduino.h>
#include <SPI.h>

#include "sd_card.h"

#include "config/definitions.h"

void SD_card::setupSD()
{
    saving = false;
    dataCount = 0;

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

void SD_card::writeSD(char buffer[][100], const char *file)
{

    myFile = SD.open(file, FILE_WRITE);

    if (myFile)
    {
        for(int i = 0; i < 1000; i++){
            Serial.println(buffer[i]);
            myFile.println(buffer[i]);
            // if(buffer[i][0] == '\n'){
            //     break;
            // }
        }

        myFile.close();
        // Serial.print("Writing to " + printFile + ": " + line);
        Serial.println("Wrote buffer to file");
        // myFile.println(line);
        // // myFile.close();
        // Serial.println("done.");
    }
    else
    {
        Serial.println("error opening " + printFile);
    }
}

