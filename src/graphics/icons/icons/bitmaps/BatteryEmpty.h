#ifndef BATTERYEMPTY_H
#define BATTERYEMPTY_H

#include "../icon.h"

class BatteryEmpty : public Icon {
    public:
        BatteryEmpty() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
