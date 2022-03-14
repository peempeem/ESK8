#include "color.h"
#include <algorithm>

/*
 * DESCRIPTION: Checks the MSB of a 15-bit color with transparency
 *              bit to determine if the color is transparent.
 * INPUTS:      c15 - 15-bit color with transparency bit
 * RETURNS:     true if the color is transparent else false
 */
bool c15IsTransparent(uint16_t c15) { return (int16_t) c15 >= 0; }

/*
 * DESCRIPTION: Converts a 15-bit color to 16 bits. The 15-bit color is
 *              in the form TRRRRRGGGGGBBBBB where T represents the transparancy
 *              bit, and R, G, B represent the 5-bit rgb channels. 16-bit color
 *              drops the transparency bit and increases the green channel to 6 bits.
 * INPUTS:      c15 - 15-bit color
 * RETURNS:     16 bit color with 5 red bits, 6 green bits, and 5 blue bits
 */
uint16_t cvtC15toC16(uint16_t c15) {
    uint16_t c16 = (c15 & 0x7C00) << 1;
    c16 += ((((c15 & 0x03E0) >> 5) * 63) / 31) << 5;
    c16 += c15 & 0x1F;
    return c16;
}

//////
//      Color CLASS FUNCTIONS  
//////

/*
 * DESCRIPTION: Default color class constructor
 */
Color::Color() {
    _r = 0;
    _g = 0;
    _b = 0;
    c16 = 0;
}

/*
 * DESCRIPTION: Color class constructor with specific rgb values
 * INPUTS:      r   - 8-bit red value
 *              g   - 8-bit green value
 *              b   = 8-bit blue value
 */
Color::Color(uint8_t r, uint8_t g, uint8_t b) {
    _r = r;
    _g = g;
    _b = b;
    c16 = (((((_r * 31) / 255) << 6) + (_g * 63) / 255) << 5) + (_b * 31) / 255;
}

/*
 * DESCRIPTION: Color class constructor from 16-bit color
 * INPUTS:      r   - 8-bit red value
 *              g   - 8-bit green value
 *              b   = 8-bit blue value
 */
Color::Color(uint16_t c16) {
    this->c16 = c16;
    _b = ((c16 & 0x1F) * 255) / 31;
    c16 = c16 >> 5;
    _g = ((c16 & 0x3F) * 255) / 63;
    c16 = c16 >> 6;
    _r = ((c16 & 0x1F) * 255) / 31;
}

/*
 * DESCRIPTION: Getter function for red channel
 * RETURNS:     red value
 */
int Color::r() { return _r; }

/*
 * DESCRIPTION: Getter function for green channel
 * RETURNS:     green value
 */
int Color::g() { return _g; }

/*
 * DESCRIPTION: Getter function for blue channel
 * RETURNS:     blue value
 */
int Color::b() { return _b; }

/*
 * DESCRIPTION: Getter function for making a copy of current color
 * RETURNS:     current color class
 */
Color Color::getColor() { return Color(_r, _g, _b); }

/*
 * DESCRIPTION: Getter function for 16-bit color
 * RETURNS:     16-bit color
 */
uint16_t Color::as16Bit() { return c16; }
 
 /*
 * DESCRIPTION: Given two colors, 
 * RETURNS:     16-bit color
 */
Color Color::blend(Color& other, float bias) {
    if (bias <= 0)
        return Color(_r, _g, _b);
    else if (bias >= 1)
        return other.getColor();
    int r = _r + (int) ((other.r() - _r) * bias);
    int g = _g + (int) ((other.g() - _g) * bias);
    int b = _b + (int) ((other.b() - _b) * bias);
    return Color(r, g, b);
}

//////
//      ColorGradient CLASS FUNCTIONS  
//////


void ColorGradient::setColors(std::vector<ColorPosition> colors) {
    if (!colors.empty())
        std::sort(colors.begin(), colors.end());
    _colors = colors;
}

Color ColorGradient::colorAtPosition(float position) {
    if (_colors.empty())
        return Color(0, 0, 0);
    int idx = -1;
    for (int i = 0; i < _colors.size(); i++) {
        if (_colors[i].position > position) {
            idx = i;
            break;
        } else if (_colors[i].position == position)
            return _colors[i].color;
    }
    if (idx == -1)
        return _colors.back().color;
    else if (idx == 0)
        return _colors[0].color;
    float diff = _colors[idx].position - _colors[idx - 1].position;
    if (diff == 0)
        return _colors[idx - 1].color;
    float bias = (position - _colors[idx - 1].position) / diff;
    return _colors[idx - 1].color.blend(_colors[idx].color, bias);
}
