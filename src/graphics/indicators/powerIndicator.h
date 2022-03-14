#ifndef POWERINDICATOR_H
#define POWERINDICATOR_H

#include "../drawable.h"

class PowerIndicator : public Indicator {
    public:
        Color borderColor = LIGHT_GRAY;
        Color lineColorDark = ALMOST_BLACK;
        Color lineColorLight = LIGHTER_GRAY;
        ColorGradient colorGradient;
        int lines = 28;
        float lineWidth = 0.7f;
        Rate pulse = Rate(0.5f);

        bool enable = false;

        PowerIndicator();
        void setPower(float battery);
        void draw(TFT_eSprite* sprite);

    protected:
        float power = 0.0f;

        float width();
        float height();
};

#endif
