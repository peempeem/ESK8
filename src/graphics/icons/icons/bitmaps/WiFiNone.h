#ifndef WIFINONE_H
#define WIFINONE_H

#include "../icon.h"

class WiFiNone : public Icon {
    public:
        WiFiNone() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
