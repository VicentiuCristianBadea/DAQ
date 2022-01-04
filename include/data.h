#ifndef DATA_H
#define DATA_H

#include "string.h"


class Data
{
    public:
        Data();

        String concatData(String);
        String getData();
        void emptyData();

    private:
        String data;
};

#endif // DATA_H
