#ifndef WIFIICON_H
#define WIFIICON_H

#include "icons/iconHandler.h"
#include "icons/bitmaps/WiFiNone.h"
#include "icons/bitmaps/WiFi1.h"
#include "icons/bitmaps/WiFi2.h"
#include "icons/bitmaps/WiFi3.h"
#include "icons/bitmaps/WiFiFull.h"

class WiFiIcon : public IconHandler {
    public:
        WiFiNone wifiEmpty;
        WiFi1 wifi1;
        WiFi2 wifi2;
        WiFi3 wifi3;
        WiFiFull wifiFull;

        Rate rate = Rate(1.5f);

        WiFiIcon() {
            addIcon(&wifiEmpty);
            addIcon(&wifi1);
            addIcon(&wifi2);
            addIcon(&wifi3);
            addIcon(&wifiFull);
            showIcon(&wifiEmpty);
        };

        void startCycle();
        void stopCycle();
        void update();
    
    private:
        bool cycling = false;
        int currentIcon;
};

#endif
