#ifndef BATTERY25_H
#define BATTERY25_H

#include "../icon.h"

class Battery25 : public Icon {
    public:
        Battery25() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
