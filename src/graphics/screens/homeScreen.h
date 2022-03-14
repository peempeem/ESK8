#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../drawable.h"
#include "boardBatteryIndicator.h"
#include "../icons/batteryIcon.h"



class HomeScreen : public Screen {
    public:
        BoardBatteryIndicator boardBatteryIndicator;
        BatteryIcon batteryIcon;

        void draw(TFT_eSprite* sprite);
};

#endif
