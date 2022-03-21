#ifndef HALL_EFFECT_H
#define HALL_EFFECT_H

#include "sensor.h"

class HallEffect : public Sensor {
    public:
        void setup(const int);
        void update();
        void setData(int);
        int getData();
        int getPin();

    private:
        int pin, data;
};


#endif