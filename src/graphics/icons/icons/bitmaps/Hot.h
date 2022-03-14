#ifndef HOT_H
#define HOT_H

#include "../icon.h"

class Hot : public Icon {
    public:
        Hot() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
