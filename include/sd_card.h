#ifndef SD_CARD_H
#define SD_CARD_H

#include <SD.h>

#include <string.h>

class SD_card
{
    public:
        void setupSD(const char *);
        void writeSD(String);

    private:
        File myFile;
        String printFile;
        int dataCount;
        bool saving;
};



#endif // SD_CARD_H
