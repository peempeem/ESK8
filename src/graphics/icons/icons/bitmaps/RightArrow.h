// THIS IS AN AUTOGENERATED FILE

#ifndef RIGHTARROW_H
#define RIGHTARROW_H

#include "../icon.h"

class RightArrow : public Icon {
    public:
        RightArrow() : Icon(32, 32) {};
        const uint16_t* getBitmap();
        void draw(TFT_eSprite* sprite);
};

#endif
