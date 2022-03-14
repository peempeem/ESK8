#ifndef RGBLIGHTS_H
#define RGBLIGHTS_H

#include "../icon.h"

class RGBLights : public Icon {
    public:
        RGBLights() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
