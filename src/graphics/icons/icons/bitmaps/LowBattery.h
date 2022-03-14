#ifndef LOWBATTERY_H
#define LOWBATTERY_H

#include "../icon.h"

class LowBattery : public Icon {
    public:
        LowBattery() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
