#ifndef BATTERYFULL_H
#define BATTERYFULL_H

#include "../icon.h"

class BatteryFull : public Icon {
    public:
        BatteryFull() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
