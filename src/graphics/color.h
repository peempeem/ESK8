#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>

bool c15IsTransparent(uint16_t c15);

uint16_t cvtC15toC16(uint16_t c15);

// 24 bit color class
class Color {
    public:
        Color();
        Color(uint8_t r, uint8_t g, uint8_t b);
        Color(uint16_t c16);

        int r();
        int g();
        int b();
        Color getColor();
        uint16_t as16Bit();
        Color blend(Color& other, float bias);

    private:
        int _r, _g, _b;
        uint16_t c16;
};

// basic colors
const static Color BLACK        (0,     0,      0   );
const static Color WHITE        (255,   255,    255 );
const static Color GRAY         (100,   100,    100 );
const static Color GREEN        (0,     255,    0   );
const static Color YELLOW       (255,   255,    0   );
const static Color RED          (255,   0,      0   );
const static Color FOREST_GREEN (0,     220,    60  );
const static Color DARK_GRAY    (20,    20,     20  );
const static Color LIGHT_GRAY   (200,   200,    200 );
const static Color LIGHTER_GRAY (145,   145,    145 );
const static Color ALMOST_BLACK (9,     5,      9   );
const static Color MAROON       (130,   0,      0   );
const static Color TURQUOISE    (64,    224,    208 );

// used in ColorGradient
struct ColorPosition {
    Color color;
    float position;

    bool operator < (const ColorPosition& other) const {
        return position < other.position;
    }

    bool operator > (const ColorPosition& other) const {
        return position > other.position;
    }

    bool operator == (const ColorPosition& other) const {
        return position == other.position;
    }
};

// blends a list of colors on a linear scale
class ColorGradient {
    public:
        ColorGradient() { }

        void setColors(std::vector<ColorPosition> colors);
        Color colorAtPosition(float position);

    private:
        std::vector<ColorPosition> colors;
};

#endif
