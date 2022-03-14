#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../drawable.h"
#include "../indicators/boardBatteryIndicator.h"
#include "../icons/batteryIcon.h"

class HomeScreen : public Screen {
    public:
        BoardBatteryIndicator boardBattery;
        BatteryIcon battery;

        void draw(TFT_eSprite* sprite);
};

#endif
