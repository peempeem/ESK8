#ifndef CONNECTSCREEN_H
#define CONNECTSCREEN_H

#include "../drawable.h"
#include "../icons/WiFiIcon.h"
#include "../icons/SkateboardIcon.h"

class ConnectScreen : public Screen {
    public:
        Rate rate = Rate(0.5f);
        Color textColor = WHITE;
        Color ringColor = WHITE;
        float circleDefault = 0.85f;
        float circleDeviation = 0.1f;
        bool visable = false;

        SkateboardIcon board;
        WiFiIcon wifi;

        void setVisability(int visability);
        void draw(TFT_eSprite* sprite);
};

#endif
