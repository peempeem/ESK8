#include "icon.h"

Icon::Icon(int width, int height) {
    _width = width;
    _height = height;
}

int Icon::width() { return _width; }

int Icon::height() { return _height; }

/*void Icon::_draw(const uint16_t* bitmap, TFT_eSprite* sprite) {
    if (!fading) {
        for (int y = 0; y < dimensions.height; y++) {
            int _y = (y * _height) / dimensions.height;
            for (int x = 0; x < dimensions.width; x++) {
                int _x = (x * _width) / dimensions.width;
                uint16_t c15 = bitmap[_width * _y + _x];
                if (!c15IsTransparent(c15)) {
                    uint16_t c16 = cvtC15toC16(c15);
                    if (blending)
                        c16 = Color(c16).blend(backgroundColor, blendBias).as16Bit();
                    sprite->drawPixel(point.x + x, point.y + y, c16);
                }
            }
        }
    } 
    else {
        for (int y = 0; y < dimensions.height; y++) {
            int _y1 = (y * _height) / dimensions.height;
            int _y2 = (y * fadeIcon->height()) / dimensions.height;
            for (int x = 0; x < dimensions.width; x++) {
                int _x1 = (x * _width) / dimensions.width;
                int _x2 = (x * fadeIcon->width()) / dimensions.width;
                uint16_t c15_1 = bitmap[_width * _y1 + _x1];
                uint16_t c15_2 = fadeIcon->getBitmap()[fadeIcon->width() * _y2 + _x2];
                bool t1 = c15IsTransparent(c15_1);
                bool t2 = c15IsTransparent(c15_2);
                if (t1 && t2)
                    continue;
                Color c;
                // fade from background to c2
                if (t1) {
                    Color c2 = Color(cvtC15toC16(c15_2));
                    c = backgroundColor.blend(c2, fadeBias);
                }
                // fade to background from c1
                else if (t2)
                    c = Color(cvtC15toC16(c15_1)).blend(backgroundColor, fadeBias);
                // fade from c1 to c2
                else {
                    Color c2 = Color(cvtC15toC16(c15_2));
                    c = Color(cvtC15toC16(c15_1)).blend(c2, fadeBias);
                }
                if (blending)
                    c = c.blend(backgroundColor, blendBias);
                sprite->drawPixel(point.x + x, point.y + y, c.as16Bit());
            }
        }
    }
}*/
