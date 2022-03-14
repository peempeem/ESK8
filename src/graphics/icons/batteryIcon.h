#ifndef BATTERYICON_H
#define BATTERYICON_H

#include "./icons/iconHandler.h"
#include "./icons/bitmaps/BatteryEmpty.h"
#include "./icons/bitmaps/Battery25.h"
#include "./icons/bitmaps/Battery50.h"
#include "./icons/bitmaps/Battery75.h"
#include "./icons/bitmaps/BatteryFull.h"

class BatteryIcon : public IconHandler {
    public:
        BatteryEmpty batteryEmpty;
        Battery25 battery25;
        Battery50 battery50;
        Battery75 battery75;
        BatteryFull batteryFull;

        BatteryIcon() {
            addIcon(&batteryEmpty);
            addIcon(&battery25);
            addIcon(&battery50);
            addIcon(&battery75);
            addIcon(&batteryFull);
            showIcon(&batteryFull);
        }
};

#endif
