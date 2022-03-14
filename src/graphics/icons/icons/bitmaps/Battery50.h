#ifndef BATTERY50_H
#define BATTERY50_H

#include "../icon.h"

class Battery50 : public Icon {
    public:
        Battery50() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
