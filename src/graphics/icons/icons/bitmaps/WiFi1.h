#ifndef WIFI1_H
#define WIFI1_H

#include "../icon.h"

class WiFi1 : public Icon {
    public:
        WiFi1() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
