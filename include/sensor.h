#ifndef SENSOR_H
#define SENSOR_H

class Sensor{
    virtual void setup(const int);
    virtual void update();
    virtual int getData();
};

#endif