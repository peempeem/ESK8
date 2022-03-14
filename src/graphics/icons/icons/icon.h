#ifndef ICON_H
#define ICON_H

#include "../../drawable.h"

class Icon {
    public:
        Dimension dimensions;
        Point point;
        Color blendColor;
        float blendBias = 0.0f;
        bool blending = false;

        Icon() {}
        Icon(int width, int height);

        int width();
        int height();
        virtual void draw(TFT_eSprite* sprite) {}
    protected:
        int _width, _height;
        void _draw(const uint16_t bitmap[], TFT_eSprite* sprite);
};

#endif
