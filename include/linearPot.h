#ifndef LINEAR_POT_H
#define LINEAR_POT_H 

#include "motor.h"

class LinearPot{
    public:
        void setupLinearPot(const int);
        int getData();

        static void updateLinearPotData(LinearPot*, LinearPot*);
        static boolean checkLinearPotDelta(LinearPot*, LinearPot*, MyMotor*, MyMotor*);

    private:
        void setData(int);
        int getPin();
        
        int pin, data;
};


#endif