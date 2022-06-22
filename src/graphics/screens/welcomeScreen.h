#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "../drawable.h"

class WelcomeScreen : public Screen {
    public:
        void setVisability(int visability);
        void draw(TFT_eSprite* sprite);

    private:
        int r = 0;
        bool growing = true;
};

#endif