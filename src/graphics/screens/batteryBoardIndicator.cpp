#include "boardBatteryIndicator.h"

BoardBatteryIndicator::BoardBatteryIndicator() {
    std::vector<ColorPosition> colors;
    colors.push_back(ColorPosition{RED, 0.2f});
    colors.push_back(ColorPosition{YELLOW, 0.5f});
    colors.push_back(ColorPosition{GREEN, 0.8f});

    colorGradient.setColors(colors);
}

float BoardBatteryIndicator::width() { return boardWidth; }

float BoardBatteryIndicator::height() { return 2 * wheelVOffset + wheelHeight; }

void BoardBatteryIndicator::setBattery(float battery) {
    if (battery < 0)
        battery = 0;
    else if (battery > 1)
        battery = 1;
    _battery = battery;
}

void BoardBatteryIndicator::draw(TFT_eSprite* sprite) {
    float scale = dimensions.width;
    float temp = scale * height();
    if ((int) temp > dimensions.height)
        scale *= dimensions.height / temp;

    // board
    float w = boardWidth * scale;
    float h = boardHeight * scale;
    float radius = boardRadius * scale;
    sprite->drawRoundRect(
        (int) (center.x - w / 2.0f),
        (int) (center.y - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        lineColor.as16Bit()
    );

    w = deckWidth * scale;
    h = deckHeight * scale;
    sprite->fillRoundRect(
        (int) (center.x - w / 2.0f),
        (int) (center.y - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        deckColor.as16Bit()
    );

    // battery bar
    w = barWidth * _battery * scale;
    h = barHeight * scale;
    float ww = (barWidth * scale) / 2.0f;
    float hh = h / 2.0f;
    radius = boardRadius * scale;
    sprite->fillRoundRect(
        (int) (center.x - ww),
        (int) (center.y - hh),
        (int) w,
        (int) h,
        (int) radius,
        colorGradient.colorAtPosition(_battery).as16Bit()
    );

    w = deckWidth * scale;
    h = deckHeight * scale;
    sprite->drawRoundRect(
        (int) (center.x - w / 2.0f),
        (int) (center.y - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        lineColor.as16Bit()
    );

    // wheels
    float hOffset = wheelHOffset * scale;
    float vOffset = wheelVOffset * scale;
    w = wheelWidth * scale;
    h = wheelHeight * scale;
    radius = wheelRadius * scale;
    sprite->drawRoundRect(
        (int) (center.x - hOffset - w / 2.0f),
        (int) (center.y - vOffset - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        lineColor.as16Bit()
    );
    sprite->drawRoundRect(
        (int) (center.x + hOffset - w / 2.0f),
        (int) (center.y - vOffset - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        lineColor.as16Bit()
    );
    sprite->drawRoundRect(
        (int) (center.x - hOffset - w / 2.0f),
        (int) (center.y + vOffset - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        lineColor.as16Bit()
    );
    sprite->drawRoundRect(
        (int) (center.x + hOffset - w / 2.0f),
        (int) (center.y + vOffset - h / 2.0f),
        (int) w,
        (int) h,
        (int) radius,
        lineColor.as16Bit()
    );
}
