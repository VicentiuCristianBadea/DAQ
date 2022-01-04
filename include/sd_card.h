#ifndef SD_CARD_H
#define SD_CARD_H

#include <SD.h>

#include <string.h>

class SD_card
{
    public:
        void setupSD();
        void writeSD(char *, String);

    private:
        File myFile;
};



#endif // SD_CARD_H
