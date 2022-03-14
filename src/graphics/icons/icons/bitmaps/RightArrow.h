#ifndef RIGHTARROW_H
#define RIGHTARROW_H

#include "../icon.h"

class RightArrow : public Icon {
    public:
        RightArrow() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
