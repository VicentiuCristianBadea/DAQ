#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H 

class DriverInput{
    public:
        void setupDriverInput(int);
        int getPin();

        static void updateDriverInput(DriverInput);

    private:
        int pin;
};



#endif 