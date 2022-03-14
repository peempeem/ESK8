#ifndef WIFIFULL_H
#define WIFIFULL_H

#include "../icon.h"

class WiFiFull : public Icon {
    public:
        WiFiFull() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
