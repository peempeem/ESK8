#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "../drawable.h"
#include "../icons/wifiIcon.h"

class WelcomeScreen : public Screen {
    public:
        Rate rate = Rate(0.5f);
        Color textColor = WHITE;
        Color ringColor = WHITE;
        float circleDefault = 0.85f;
        float circleDeviation = 0.1f;

        WiFiIcon wifi;

        void draw(TFT_eSprite* sprite);
};

#endif
