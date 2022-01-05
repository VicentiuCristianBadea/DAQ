#ifndef DATA_H
#define DATA_H

#include "string.h"


class Data
{
    public:
        Data();

        void concatData(String);
        void emptyData();
        
        String getData();
        

    private:
        String data;
};

#endif // DATA_H
