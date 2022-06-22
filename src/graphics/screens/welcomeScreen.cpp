#include "welcomeScreen.h"

void WelcomeScreen::setVisability(int visability) {

}

void WelcomeScreen::draw(TFT_eSprite* sprite) {
    int w = dimensions.width / 2;
    int h = dimensions.height / 2;
    float b = (h - r) / (float) h;
    Color c1 = SUNSET_PURPLE;
    Color c2 = SUNSET_ORANGE;
    sprite->drawCircle(w, h, r, c1.blend(c2, b).as16Bit());
    r += 7;
}