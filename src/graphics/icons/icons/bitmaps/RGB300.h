// THIS IS AN AUTOGENERATED FILE

#ifndef RGB300_H
#define RGB300_H

#include "../icon.h"

class RGB300 : public Icon {
    public:
        RGB300() : Icon(32, 32) {};
        const uint16_t* getBitmap();
        void draw(TFT_eSprite* sprite);
};

#endif
