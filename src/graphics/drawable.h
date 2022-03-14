#ifndef SCREENS_H
#define SCREENS_H

#include "../rate.h"
#include "color.h"
#include <TFT_eSPI.h>

struct Point { int x, y; };
struct FPoint { float x, y; };
struct Dimension { int width, height; };

class Screen {
    public:
        Dimension dimensions;
        Point point;
        Color backgroundColor = BLACK;
        Color cornerColor = GRAY;
        Color dotsDefaultColor = LIGHT_GRAY;
        Color dotsBlendColor = ALMOST_BLACK;
        float cornerSize = 0.1f;
        int dots = 15;
        Rate dotsRate = Rate(0.4f);

        virtual void draw(TFT_eSprite* sprite) {}
        void drawBackground(TFT_eSprite* sprite);
};

class Indicator {
    public:
        Dimension dimensions;
        FPoint center;

        virtual void draw(TFT_eSprite* sprite);

    protected:
        virtual float width();
        virtual float height();
};

#endif
