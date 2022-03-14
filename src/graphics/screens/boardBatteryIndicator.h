#ifndef BOARDBATTERYINDICATOR_H
#define BOARDBATTERYINDICATOR_H

#include "../drawable.h"

class BoardBatteryIndicator : public Indicator {
    public:
        Color lineColor = WHITE;
        Color deckColor = DARK_GRAY;
        ColorGradient colorGradient;
        float boardWidth = 1.0f;
        float boardHeight = 0.17f;
        float boardRadius = 0.05f;
        float deckWidth = 0.7f;
        float deckHeight = 0.24;
        float barWidth = 0.65f;
        float barHeight = 0.19f;
        float wheelHOffset = 0.4f;
        float wheelVOffset = 0.14f;
        float wheelWidth = 0.1f;
        float wheelHeight = 0.07f;
        float wheelRadius = 0.02f;

        BoardBatteryIndicator();
        void setBattery(float battery);
        void draw(TFT_eSprite* sprite);

    protected:
        float _battery = 0.0f;

        float width();
        float height();
};

#endif
