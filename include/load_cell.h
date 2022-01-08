#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include <string.h>
#include <HX711.h>

#include "data.h"


class LoadCell
{

    public:

        void setupLoadCell(int, int, int);
        void loadCellBegin();
        void calibrate();
        double readLoadDouble();
        String readLoadString();


    private:

        int _dout_pin;
        int _sck_pin;
        int _timeout;
        int _timer;
        float _calibration_factor;

        HX711 _scale;

};

#endif // LOAD_CELL_H
