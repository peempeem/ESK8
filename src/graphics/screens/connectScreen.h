#ifndef CONNECTSCREEN_H
#define CONNECTSCREEN_H

#include "../drawable.h"
#include "../icons/WiFiIcon.h"
#include "../icons/SkateboardIcon.h"

class ConnectScreen : public Screen {
    public:
        Color textColor = GREEN;
        String text;

        SkateboardIcon board;
        WiFiIcon wifi;

        void setVisability(int visability);
        void draw(TFT_eSprite* sprite);
};

#endif
