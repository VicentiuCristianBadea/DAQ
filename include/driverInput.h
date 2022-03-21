#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H 

class DriverInput{
    public:
        void setup(const int);
        int getPin();
        void update();
        
    private:
        int pin;
};



#endif 