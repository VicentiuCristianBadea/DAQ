#ifndef LINEAR_POT_H
#define LINEAR_POT_H 

class LinearPot{
    public:
        void setupLinearPot(const int);
        int getData();

        static void updateLinearPotData(LinearPot*, LinearPot*);
        static boolean checkLinearPotDelta(LinearPot*, LinearPot*);

    private:
        void setData(int);
        int getPin();
        
        int pin, data;
};


#endif