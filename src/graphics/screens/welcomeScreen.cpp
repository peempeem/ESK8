#include "welcomeScreen.h"

void WelcomeScreen::draw(TFT_eSprite* sprite) {
    drawBackground(sprite);
    float stage = rate.getStageSin();
    float circleSize = circleDeviation * stage + circleDefault;
    Point center = {dimensions.width / 2, dimensions.height / 2};
    sprite->setFreeFont(&Orbitron_Light_24);
    sprite->setTextDatum(MC_DATUM);
    sprite->setTextColor(textColor.as16Bit());
    sprite->drawString(
        text.c_str(),
        center.x,
        center.y,
        1
    );
    sprite->drawCircle(
        center.x,
        center.y,
        circleSize * center.y,
        ringColor.as16Bit()
    );
}
