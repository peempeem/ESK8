#ifndef PWMLED_H
#define PWMLED_H

#include "../util/rate.h"

class PWMLed {
    public:
        Rate blinkRate;

        PWMLed();

        void init(int pin);
        void start_blinking();
        void stop_blinking();
        void turn_on();
        void turn_off();


    private:
        bool blinking = false;
};

#endif