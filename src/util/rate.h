#ifndef RATE_H
#define RATE_H

#include <Arduino.h>

class Rate {
    public:
        bool enabled = true;

        Rate() { enabled = false; };
        Rate(float rate);

        void setRate(float rate);
        bool isReady();

        float getStage();
        float getStageSin() { return sin(getStage() * 2 * PI); }
        float getStageNCos() { return -cos(getStage() * 2 * PI); }

    private:
        int inverseRate = 0;
        int last = 0;
};

#endif
