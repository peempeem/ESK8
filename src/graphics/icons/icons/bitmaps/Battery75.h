#ifndef BATTERY75_H
#define BATTERY75_H

#include "../icon.h"

class Battery75 : public Icon {
    public:
        Battery75() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
