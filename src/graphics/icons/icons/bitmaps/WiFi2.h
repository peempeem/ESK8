#ifndef WIFI2_H
#define WIFI2_H

#include "../icon.h"

class WiFi2 : public Icon {
    public:
        WiFi2() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif