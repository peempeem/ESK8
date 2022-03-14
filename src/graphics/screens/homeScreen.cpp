#include "homeScreen.h"

void HomeScreen::draw(TFT_eSprite* sprite) {
    drawBackground(sprite);
    boardBattery.dimensions.width = dimensions.width * 0.75f;
    boardBattery.dimensions.height = dimensions.height * 0.2f;
    boardBattery.center.x = dimensions.width * 0.625f;
    boardBattery.center.y = dimensions.height * 0.9f;
    boardBattery.draw(sprite);
    battery.dimensions = Dimension{128, 128};
    battery.point = Point{0, 0};
    battery.draw(sprite);
}
