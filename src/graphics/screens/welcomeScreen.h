#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "../drawable.h"

class WelcomeScreen : public Screen {
    public:
        Rate rate = Rate(0.5f);
        Color textColor = WHITE;
        Color ringColor = WHITE;
        String text = "Hello!";
        float circleDefault = 0.85f;
        float circleDeviation = 0.1f;

        void draw(TFT_eSprite* sprite);
};

#endif
