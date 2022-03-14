#ifndef BATTERYICON_H
#define BATTERYICON_H

#include "icons/iconHandler.h"
#include "icons/bitmaps/BatteryEmpty.h"
#include "icons/bitmaps/Battery25.h"
#include "icons/bitmaps/Battery50.h"
#include "icons/bitmaps/Battery75.h"
#include "icons/bitmaps/BatteryFull.h"

class BatteryIcon : public IconHandler {
    public:
        BatteryEmpty empty;
        Battery25 _25;
        Battery50 _50;
        Battery75 _75;
        BatteryFull full;

        BatteryIcon() {
            addIcon(&empty);
            addIcon(&_25);
            addIcon(&_50);
            addIcon(&_75);
            addIcon(&full);
            showIcon(&full);
        }
};

#endif
