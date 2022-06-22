#ifndef BATTERYICON_H
#define BATTERYICON_H

#include "iconHandler.h"

class BatteryIcon : public IconHandler {
    public:
        Icon empty = Icon("/icons/BatteryEmpty.icon");
        Icon _25 = Icon("/icons/Battery25.icon");
        Icon _50 = Icon("/icons/Battery50.icon");
        Icon _75 = Icon("/icons/Battery75.icon");
        Icon full = Icon("/icons/BatteryFull.icon");;

        BatteryIcon() {
            std::vector<Icon*> icons = {
                &empty,
                &_25,
                &_50,
                &_75,
                &full
            };
            setIcons(icons);
            showIcon(&full);
        }
};

#endif