#include "welcomeScreen.h"

void WelcomeScreen::draw(TFT_eSprite* sprite) {
    drawBackground(sprite);
    float stage = rate.getStageSin();
    float circleSize = circleDeviation * stage + circleDefault;
    Point center = {dimensions.width / 2, dimensions.height / 2};
    sprite->drawCircle(
        center.x,
        center.y,
        circleSize * center.y,
        ringColor.as16Bit()
    );

    wifi.dimensions.width = dimensions.width * 0.8f;
    wifi.dimensions.height = dimensions.width * 0.8f;
    wifi.point.x = dimensions.width * 0.1f;
    wifi.point.y = (dimensions.height - wifi.dimensions.height) / 2;
    wifi.draw(sprite);
}
