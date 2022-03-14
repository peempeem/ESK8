#ifndef LEFTARROW_H
#define LEFTARROW_H

#include "../icon.h"

class LeftArrow : public Icon {
    public:
        LeftArrow() : Icon(32, 32) {};
        void draw(TFT_eSprite* sprite);
};

#endif
