#include "homeScreen.h"

void HomeScreen::draw(TFT_eSprite* sprite) {
    drawBackground(sprite);
    /*boardBatteryIndicator.dimensions.width = dimensions.width * 0.75f;
    boardBatteryIndicator.dimensions.height = dimensions.height * 0.2f;
    boardBatteryIndicator.center.x = dimensions.width * 0.625f;
    boardBatteryIndicator.center.y = dimensions.height * 0.9f;
    boardBatteryIndicator.draw(sprite);*/
    batteryIcon.dimensions = Dimension{128, 129};
    batteryIcon.point = Point{0, 0};
    batteryIcon.draw(sprite);
}
