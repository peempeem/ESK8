#include "icon.h"

Icon::Icon(int width, int height) {
    _width = width;
    _height = height;
}

int Icon::width() { return _width; }

int Icon::height() { return _height; }

void Icon::_draw(const uint16_t bitmap[], TFT_eSprite* sprite) {
    for (int y = 0; y < dimensions.height; y++) {
        int _y = (y * _height) / dimensions.height;
        for (int x = 0; x < dimensions.width; x++) {
            int _x = (x * _width) / dimensions.width;
            uint16_t c15 = bitmap[_width * _y + _x];
            if (!c15IsTransparent(c15)) {
                uint16_t c16 = cvtC15toC16(c15);
                if (blending)
                    c16 = Color(c16).blend(blendColor, blendBias).as16Bit();
                sprite->drawPixel(point.x + x, point.y + y, c16);
            }
        }
    }
}
