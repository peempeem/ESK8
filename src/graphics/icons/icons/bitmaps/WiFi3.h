// THIS IS AN AUTOGENERATED FILE

#ifndef WIFI3_H
#define WIFI3_H

#include "../icon.h"

class WiFi3 : public Icon {
    public:
        WiFi3() : Icon(32, 32) {};
        const uint16_t* getBitmap();
        void draw(TFT_eSprite* sprite);
};

#endif
