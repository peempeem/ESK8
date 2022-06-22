#include "homeScreen.h"

void HomeScreen::setVisability(int visability) {
    
}

void HomeScreen::draw(TFT_eSprite* sprite) {
    drawBackground(sprite);

    boardBattery.dimensions.width = dimensions.width * 0.7f;
    boardBattery.dimensions.height = dimensions.height * 0.2f;
    boardBattery.center.x = boardBattery.dimensions.width / 2 + dimensions.width * 0.3f;
    boardBattery.center.y = boardBattery.dimensions.height / 2 + dimensions.height * 0.8f;
    boardBattery.draw(sprite);

    power.dimensions.height = dimensions.height * 0.9f;
    power.dimensions.width = dimensions.width * 0.2f;
    power.center.x = power.dimensions.width / 2 + dimensions.width * 0.05;
    power.center.y = power.dimensions.height / 2 + dimensions.height * 0.05;
    power.draw(sprite);

    sprite->setFreeFont(&Orbitron_Light_32);
    sprite->setTextDatum(MC_DATUM);
    sprite->setTextColor(speedColor.as16Bit());
    Point center {
        boardBattery.center.x,
        dimensions.height * 0.15f
    };
    sprite->drawString(
        speed.c_str(),
        center.x,
        center.y,
        1
    );

    sprite->setFreeFont(&Orbitron_Light_24);
    sprite->setTextColor(speedDescColor.as16Bit());
    center.y = dimensions.height * 0.3f;
    sprite->drawString(
        speedDesc.c_str(),
        center.x,
        center.y,
        1
    );
}

void HomeScreen::setSpeed(float speed) { this->speed = String(fabs(speed), 1); }
