#include "connectScreen.h"

void ConnectScreen::setVisability(int visability) {
    switch (visability) {
        case VISABLE:
            wifi.allocate();
            board.allocate();
            break;
        case INVISABLE: 
            wifi.deallocate();
            board.deallocate();
            break;
    }
}

void ConnectScreen::draw(TFT_eSprite* sprite) {
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

    wifi.dimensions.width = dimensions.width * 0.5f;
    wifi.dimensions.height = wifi.dimensions.width;
    wifi.point.x = center.x - wifi.dimensions.width / 2;
    wifi.point.y = (dimensions.height - wifi.dimensions.height) / 2 - 25;
    wifi.draw(sprite);

    board.dimensions.width = dimensions.width * 0.95f;
    board.dimensions.height = board.dimensions.width * (board.currentIcon()->height() / (float) board.currentIcon()->width());
    board.point.x = center.x - board.dimensions.width / 2;
    board.point.y = 25 + (dimensions.height - board.dimensions.height) / 2;
    board.draw(sprite);
}
