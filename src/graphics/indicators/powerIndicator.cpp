#include "powerIndicator.h"

PowerIndicator::PowerIndicator() {
    std::vector<ColorPosition> colors;
    colors.push_back(ColorPosition{RED, 0});
    colors.push_back(ColorPosition{YELLOW, 0.5f});
    colors.push_back(ColorPosition{GREEN, 1});
    colorGradient.setColors(colors);
}

float PowerIndicator::width() { return 1; }

float PowerIndicator::height() { return 1; }

void PowerIndicator::setPower(float power) {
    if (power < 0)
        power = 0;
    else if (power > 1)
        power = 1;
    this->power = power;
}

void PowerIndicator::draw(TFT_eSprite* sprite) {
    float w = dimensions.width;
    float h = dimensions.height;
    float radius = 3;
    sprite->drawRoundRect(
        (int) (center.x - w / 2.0f),
        (int) (center.y - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        borderColor.as16Bit()
    );

    float scale = dimensions.height / (float) lines;
    FPoint p {
        center.x - lineWidth * dimensions.width / 2.0f, 
        center.y - dimensions.height / 2.0f + scale / 2.0f
    };

    if (!enable) {
        float stage = pulse.getStage();
        for (int i = 0; i < lines; i++) {
            float bias = (sin((stage + i / (float) (lines - 1)) * 2 * PI) + 1) / 2.0f;
            sprite->drawFastHLine(
                (int) p.x, 
                (int) p.y, 
                lineWidth * dimensions.width, 
                lineColorDark.blend(lineColorLight, bias).as16Bit()
            );
            p.y += scale;
        }
    } else {
        uint16_t c16 = colorGradient.colorAtPosition(power).as16Bit();
        for (int i = 0; i < lines; i++) {
            float pos = 1 - i / (float) (lines - 1);
            if (power >= 0.5f && pos >= 0.5f && pos <= power) {
                sprite->drawFastHLine(
                    (int) p.x, 
                    (int) p.y, 
                    lineWidth * dimensions.width, 
                    c16
                );
            } else if (power <= 0.5f && pos <= 0.5f && pos >= power) {
                sprite->drawFastHLine(
                    (int) p.x, 
                    (int) p.y, 
                    lineWidth * dimensions.width, 
                    c16
                );
            }
            p.y += scale;
        }
    }
}
